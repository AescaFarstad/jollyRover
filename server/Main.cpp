
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <GameUpdater.h>
#include <Prototypes.h>
#include <GameStateMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <GenericRequestMessage.h>
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
}

template <typename T>
void saveBinary(const T& object, const std::string name)
{
	std::string fileName = "../dumps/" + name + ".binary";
	BinarySerializer b;
	b.write(object);
	auto data = b.dumpAll();
	auto file = std::fstream(fileName, std::ios::out | std::ios::binary);
	file.write((char*)&data[0], data.size());
	file.close();
	S::log.add("Write file " + fileName + "(" + b.crc() + ")");
}

#include <AI.h>
#include <Cars.h>
SeededRandom rnd(5254534);
int32_t cooldown = 4000;
void tryToFakeInput()
{
	cooldown--;
	if (cooldown < 0 && gameUpdater.state.players.size() > 0 && rnd.get() > 0.9996)
	{
		auto& player = rnd.getFromVector(gameUpdater.state.players);
		if (!Cars::canLaunchCar(player))
			return;
		auto inmsg = std::make_unique<InputRouteMessage>();
		inmsg->login = player.login;
		auto rnd = gameUpdater.state.random;
		inmsg->route = AI::getRandomWalk(rnd, &prototypes);
		handleNetworkMessage(std::move(inmsg));
		cooldown = 1000;
	}
}

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
	
	tryToFakeInput();
	
	if (delta >= MIN_TIME_PER_FRAME)
	{
		lastTicks = ticks;
		gameUpdater.update(ticks);
	}
	
	if (!network.hasClients())
		timeWithoutClients += delta;
	else
		timeWithoutClients = 0;
	
	if (timeWithoutClients > prototypes.variables.reconnectWindow * 2)
	{
		idle = true;
		S::log.add("Went idle");
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
		S::log.add("state reset");
		
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

int main()
{
	printf(".\n");

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	loadPrototypes();
	loadConfig();
	
	network.init(
		[](int32_t login) -> bool {return GameLogic::playerByLogin(&gameUpdater.state, login) != nullptr;},
		[](int32_t login) -> bool {return GameLogic::playerByLogin(&gameUpdater.state, login) != nullptr;},
		&prototypes.variables
		);	
		
	gameUpdater.load(GameState(), &prototypes, false);

	while (!isFinished)
		mainLoop();

	SDLNet_Quit();
	SDL_Quit();

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
			InputMessage* t = dynamic_cast<InputMessage*>(message.release());
			std::unique_ptr<InputMessage> iMsg = std::unique_ptr<InputMessage>(t);
			iMsg->serverId = inputIdCounter++;
			iMsg->serverStamp = gameUpdater.state.timeStamp;
			network.sendToAllPlaying(*iMsg);
			gameUpdater.addNewInput(std::move(iMsg));
			break;
		}
		case MESSAGE_TYPE::TYPE_CHECKSUM_MSG:
		{
			ChecksumMessage* t = dynamic_cast<ChecksumMessage*>(message.release());
			std::unique_ptr<ChecksumMessage> crcMsg = std::unique_ptr<ChecksumMessage>(t);
			//S::log.add("Analyzing checksums. count=" + std::to_string(crcMsg->checksums.size()), {LOG_TAGS::ERROR_});
			auto mismatch = gameUpdater.crcs.getMismatch(crcMsg->checksums);
			if (mismatch.has_value())
			{
				S::log.add("Desync detected for client " + std::to_string(crcMsg->login) + " at stamp " + std::to_string(mismatch.value()), {LOG_TAGS::ERROR_});
				
				StateRequestMessage srMsg;
				srMsg.states.push_back(mismatch.value() - prototypes.variables.fixedStepDuration);
				srMsg.states.push_back(mismatch.value());
				
				auto client = network.clientByLogin(crcMsg->login);
				if (client->requestedStates.size() > 0)
				{
					S::log.add("Client " + std::to_string(crcMsg->login) + " is already in desync, awaiting requested states", {LOG_TAGS::ERROR_});
					break;
				}
				
				client->requestedStates = srMsg.states;
				
				network.send(srMsg, crcMsg->login);
			}
			break;
		}
		case MESSAGE_TYPE::TYPE_GAME_STATE_MSG:
		{
			GameStateMessage* t = dynamic_cast<GameStateMessage*>(message.release());
			std::unique_ptr<GameStateMessage> msg = std::unique_ptr<GameStateMessage>(t);
			
			auto client = network.clientByLogin(msg->login);
			if (client->requestedStates.size() != 2)
			{
				S::log.add("Client " + std::to_string(msg->login) + " has sent states without a proper request", {LOG_TAGS::ERROR_});
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
						S::log.add("Failed to fetch server state to check desync at " + std::to_string(state.timeStamp), {LOG_TAGS::ERROR_});
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
					S::log.add("Client " + std::to_string(msg->login) + " has sent state at " + std::to_string(state.timeStamp) + " though it wasn't among the requested", {LOG_TAGS::ERROR_});
				}
			}
			client->requestedStates.clear();
			break;
		}
		case MESSAGE_TYPE::TYPE_REQUEST_MSG:
		{
			GenericRequestMessage* t = dynamic_cast<GenericRequestMessage*>(message.get());
			std::unique_ptr<GenericRequestMessage> genericRequestMsg = std::make_unique<GenericRequestMessage>(*t);
			switch (genericRequestMsg->request)
			{
				case REQUEST_TYPE::REQUEST_JOIN_GAME:
				{
					network.addPlayer(genericRequestMsg->login);

					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;					
					
					BinarySerializer::copyThroughSerialization(gameUpdater.state, gsMsg.states.emplace_back());
										
					//S::log.add("Send state, stamp=" + std::to_string(gameUpdater.state.timeStamp) + " players=" + std::to_string(gameUpdater.state.players.size()) + " crc=" + BinarySerializer::crc(gsMsg.states[0]), {LOG_TAGS::SUBTASK});
					network.send(gsMsg, genericRequestMsg->login);

					std::unique_ptr<InputPlayerJoinedMessage> pjMsg = std::make_unique<InputPlayerJoinedMessage>();
					pjMsg->serverId = inputIdCounter++;
					pjMsg->serverStamp = gameUpdater.state.timeStamp;
					pjMsg->login = genericRequestMsg->login;
					//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					network.sendToAllPlaying(*pjMsg);
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
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}


