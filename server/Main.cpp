
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <std2.h>
#include <GameUpdater.h>
#include <DemoWriter.h>
#include <Prototypes.h>
#include <GameStateMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <GenericRequestMessage.h>
#include <DemoDataMessage.h>
#include <DemoListMessage.h>
#include <DemoRequestMessage.h>
#include <NetworkPacket.h>
#include <ServerNetwork.h>
#include <ChecksumMessage.h>
#include <StateRequestMessage.h>
#include <JSONSerializer.h>
#undef main

namespace MainInternal 
{
	const int32_t MIN_TIME_PER_FRAME = 10;

	bool isFinished = false;
	int32_t lastTicks = 0;
	ServerNetwork network;	
	MessageBuffer messageBuffer;
	GameUpdater gameUpdater;
	int32_t inputIdCounter = 0;
	Prototypes prototypes;
	bool hadClients = false;
	int32_t timeWithoutClients = 0;
	bool idle = false;
	int32_t filesWritten = 0;
	DemoWriter demoWriter;
}

using namespace MainInternal;

void handleNetworkMessage(std::unique_ptr<NetworkMessage> message);

void loadPrototypes()
{
	std::ifstream file("out/prototypes.json");
	nlohmann::json j = nlohmann::json::parse(file);
	file.close();
	prototypes.load(j);
}

void loadConfig()
{
	std::ifstream file("out/config.json");
	nlohmann::json j = nlohmann::json::parse(file);
	file.close();
	S::config.load(j);
	//S::config.saveStateInterval = -1; //Server never needs to save states. Unless it checks for desyncs
}

template <typename T>
void saveJSON(const T& object, const std::string name)
{
	std::string fileName = "../dumps/" + name + ".txt";
	JSONSerializer j;
	j.write(object);
	std::ofstream file(fileName);
	file << j.toString();
	file.close();
	S::log.add("Write file " + fileName);
	filesWritten++;
}

template <typename T>
void saveBinary(const T& object, const std::string name)
{
	std::string fileName = "../dumps/" + name + ".binary";
	BinarySerializer b;
	b.write(object);
	auto data = b.dumpAll();
	auto file = std::fstream(fileName, std::ios::out | std::ios::binary);
	file.write(data.data(), data.size());
	file.close();
	S::log.add("Write file " + fileName + "(" + b.crc() + ")");
}

std::string getCurrentTimeString()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%S-%M-%H [%d-%m]");
	return oss.str();
};

void activeLoop()
{
	int32_t ticks = SDL_GetTicks();
	int32_t delta = ticks - lastTicks;
	
	int32_t i = 0;
	while (messageBuffer[i] != nullptr)
	{
		handleNetworkMessage(std::move(messageBuffer[i]));
		messageBuffer[i] = nullptr;
		i++;
	}
	
	if (delta < MIN_TIME_PER_FRAME)
		return;
	
	lastTicks = ticks;
	gameUpdater.update(ticks);
	
	if (!network.hasClients())
		timeWithoutClients += delta;
	else
		timeWithoutClients = 0;
	
	if (timeWithoutClients > prototypes.variables.reconnectWindow * 2)
	{
		idle = true;
		demoWriter.end();
		S::log.add("Went idle", {LOG_TAGS::HARD_LOG});
	}	
}

void idleLoop()
{
	if (network.hasClients())
	{
		idle = false;
		
		
		int32_t ticks = SDL_GetTicks() - prototypes.variables.fixedStepDuration;
		GameState state;
		state.timeStamp = ticks;
		lastTicks = ticks;
		gameUpdater.load(state, &prototypes, false);
		
		demoWriter.start(getCurrentTimeString());
		demoWriter.write(state);
		S::log.add("Went active", {LOG_TAGS::HARD_LOG});
		activeLoop();
	}
}

void mainLoop()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			SDL_Quit();
			isFinished = true;
			break;
		}
	}
	
	network.update(messageBuffer);
	if (idle)
		idleLoop();
	else
		activeLoop();
}

void initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	loadPrototypes();
	loadConfig();
	
	if (S::config.enableHardLog)
	{
		auto logName = S::config.hardLogPath + getCurrentTimeString() + ".txt";
		S::log = Logger(S::log.enabledTags, S::log.disabledTags, [logName](uint32_t stamp, const std::string& message){
						
			std::ofstream log(logName, std::ios_base::app | std::ios_base::out);
			auto tabbedMessage = message;
			std2::replaceAll(tabbedMessage, "\n", "\n\t");
			log << getCurrentTimeString() << "\t" << stamp  << "\n\t" << tabbedMessage << "\n";
		});	
		
	}
	S::log.add("Server started", {LOG_TAGS::HARD_LOG});
	
	network.init(
		[](int32_t login) -> bool {return GameLogic::playerByLogin(&gameUpdater.state, login) != nullptr;},
		[](int32_t login) -> bool {return GameLogic::playerByLogin(&gameUpdater.state, login) != nullptr;},
		&prototypes.variables
		);
		
	demoWriter.start("default");
}

void quitSystems()
{
	S::log.add("End", {LOG_TAGS::HARD_LOG});
	SDLNet_Quit();
	SDL_Quit();
}

int main()
{	
	initSystems();
	
	gameUpdater.load(GameState(), &prototypes, false);

	while (!isFinished)
		mainLoop();

	quitSystems();

	return 0;
}

void handleNetworkMessage(std::unique_ptr<NetworkMessage> message)
{
	switch (message->typeId)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG:
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG:
		{
			
			S::log.add("Client input " + message->getName() + " from " + std::to_string(message->login), {LOG_TAGS::HARD_LOG});
			
			auto iMsg = std2::unique_ptr_cast<InputMessage>(std::move(message));
			iMsg->serverId = inputIdCounter++;
			iMsg->serverStamp = gameUpdater.state.timeStamp;
			network.sendToAllPlaying(*iMsg);
			demoWriter.write(*iMsg);
			gameUpdater.addNewInput(std::move(iMsg));
			break;
		}
		case MESSAGE_TYPE::TYPE_CHECKSUM_MSG:
		{
			auto crcMsg = std2::unique_ptr_cast<ChecksumMessage>(std::move(message));
			auto client = network.clientByLogin(crcMsg->login);
			if (client->hasDesynced)
				break;
			auto mismatch = gameUpdater.crcs.getMismatch(crcMsg->checksums);
			if (mismatch.has_value())
			{
				client->hasDesynced = true;
				S::log.add("Desync detected for client " + std::to_string(crcMsg->login) + " at stamp " + std::to_string(mismatch.value()), {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
				
				StateRequestMessage srMsg;
				srMsg.states.push_back(mismatch.value() - prototypes.variables.fixedStepDuration);
				srMsg.states.push_back(mismatch.value());
				
				if (client->requestedStates.size() > 0)
				{
					S::log.add("Client " + std::to_string(crcMsg->login) + " is already in desync, awaiting requested states", {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
					break;
				}
				
				client->requestedStates = srMsg.states;								
				
				network.send(srMsg, crcMsg->login);
			}
			break;
		}
		case MESSAGE_TYPE::TYPE_GAME_STATE_MSG:
		{
			auto msg = std2::unique_ptr_cast<GameStateMessage>(std::move(message));
			
			auto client = network.clientByLogin(msg->login);
			if (client->requestedStates.size() != 2)
			{
				S::log.add("Client " + std::to_string(msg->login) + " has sent states without a proper request", {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
				break;
			}
			
			for(auto& state : msg->states)
			{
				if (state.timeStamp == client->requestedStates[0])
				{
					std::string fileName = "desync_c-" + std::to_string(msg->login) + "_t-" + std::to_string(state.timeStamp) + "_crc-" + BinarySerializer::crc(state) + "_synced";
					saveBinary(state, fileName);
					saveJSON(state, fileName);
				}
				else if (state.timeStamp == client->requestedStates[1])
				{
					auto serverState = gameUpdater.getSavedStateByStamp(state.timeStamp);
					if (!serverState.has_value())
					{
						S::log.add("Failed to fetch server state to check desync at " + std::to_string(state.timeStamp), {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
					}
					std::string fileName = "desync_c-" + std::to_string(msg->login) + "_t-" + std::to_string(state.timeStamp);
					
					auto cName = fileName + "_crc-" + BinarySerializer::crc(state) + "_client";
					saveBinary(state, cName);
					saveJSON(state, cName);
					
					auto sName = fileName + "_crc-" + BinarySerializer::crc(serverState.value()) + "_server";
					saveBinary(serverState.value(), sName);
					saveJSON(serverState.value(), sName);
				}
				else
				{
					S::log.add("Client " + std::to_string(msg->login) + " has sent state at " + std::to_string(state.timeStamp) + " though it wasn't among the requested", {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
				}
			}
			client->requestedStates.clear();
			break;
		}
		case MESSAGE_TYPE::TYPE_DEMO_REQUEST:
		{
			auto demoRequest = std2::unique_ptr_cast<DemoRequestMessage>(std::move(message));
			
			DemoDataMessage ddm;
			ddm.data = demoWriter.getDemo(demoRequest->demoName);
			ddm.inResponseTo = demoRequest->initiator_id;
			BinarySerializer bs;
			ddm.serialize(bs);
			network.send(ddm, demoRequest->login);
			break;
		}
		case MESSAGE_TYPE::TYPE_REQUEST_MSG:
		{
			auto genericRequestMsg = std2::unique_ptr_cast<GenericRequestMessage>(std::move(message));
			
			switch (genericRequestMsg->request)
			{
				case REQUEST_TYPE::REQUEST_JOIN_GAME:
				{
					network.addPlayer(genericRequestMsg->login);

					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;					
					
					BinarySerializer::copyThroughSerialization(gameUpdater.state, gsMsg.states.emplace_back());
										
					network.send(gsMsg, genericRequestMsg->login);

					std::unique_ptr<InputPlayerJoinedMessage> pjMsg = std::make_unique<InputPlayerJoinedMessage>();
					pjMsg->serverId = inputIdCounter++;
					pjMsg->serverStamp = gameUpdater.state.timeStamp;
					pjMsg->login = genericRequestMsg->login;
					//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					network.sendToAllPlaying(*pjMsg);
					demoWriter.write(*pjMsg);
					gameUpdater.addNewInput(std::move(pjMsg));
					break;
				}
				case REQUEST_TYPE::REQUEST_GAME_STATE:
				{
					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					BinarySerializer::copyThroughSerialization(gameUpdater.state, gsMsg.states.emplace_back()); 
					network.send(gsMsg, genericRequestMsg->login);
					break;
				}
				case REQUEST_TYPE::REQUEST_PING:
				{
					GenericRequestMessage grMsg;
					grMsg.request = REQUEST_TYPE::REQUEST_PONG;
					grMsg.inResponseTo = genericRequestMsg->initiator_id;
					network.send(grMsg, genericRequestMsg->login);
					break;
				}
				case REQUEST_TYPE::REQUEST_DEMO_LIST:
				{
					DemoListMessage dlm;
					dlm.list = demoWriter.getList();
					dlm.inResponseTo = genericRequestMsg->initiator_id;
					network.send(dlm, genericRequestMsg->login);
					break;
				}
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}


