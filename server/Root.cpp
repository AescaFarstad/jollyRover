#include <Root.h>
#include <GameStateMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <GenericRequestMessage.h>
#include <StateRequestMessage.h>
#include <DemoDataMessage.h>
#include <DemoListMessage.h>
#include <DemoRequestMessage.h>
#include <ChecksumMessage.h>
#include <NetworkPacket.h>
#include <JSONSerializer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <std2.h>

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
	file.write(data.data(), data.size());
	file.close();
	S::log.add("Write file " + fileName + "(" + b.crc() + ")");
}

void Root::init()
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
	
	m_network.init(
		[this](int32_t login) -> bool {return GameLogic::playerByLogin(&m_gameUpdater.state, login) != nullptr;},
		[this](int32_t login) -> bool {return GameLogic::playerByLogin(&m_gameUpdater.state, login) != nullptr;},
		&m_prototypes.variables
		);
		
	m_demoWriter.start("default");
	
	m_gameUpdater.load(GameState(), &m_prototypes, false);
	
}

bool Root::mainLoop()
{
	bool isFinished = false;
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
	
	m_network.update(m_messageBuffer);
	if (m_isIdle)
		idleLoop();
	else
		activeLoop();
	
	return isFinished;
}

void Root::activeLoop()
{
	int32_t ticks = SDL_GetTicks();
	int32_t delta = ticks - m_lastTicks;
	
	int32_t i = 0;
	while (m_messageBuffer[i] != nullptr)
	{
		handleNetworkMessage(std::move(m_messageBuffer[i]));
		m_messageBuffer[i] = nullptr;
		i++;
	}
	
	if (delta < MIN_TIME_PER_FRAME)
		return;
	
	m_lastTicks = ticks;
	m_gameUpdater.update(ticks);
	
	if (!m_network.hasClients())
		m_timeWithoutClients += delta;
	else
		m_timeWithoutClients = 0;
	
	if (m_timeWithoutClients > m_prototypes.variables.reconnectWindow * 2)
	{
		m_isIdle = true;
		m_demoWriter.end();
		S::log.add("Went idle", {LOG_TAGS::HARD_LOG});
	}	
}

void Root::idleLoop()
{
	if (m_network.hasClients())
	{
		m_isIdle = false;
		
		
		int32_t ticks = SDL_GetTicks() - m_prototypes.variables.fixedStepDuration;
		GameState state;
		state.timeStamp = ticks;
		m_lastTicks = ticks;
		m_gameUpdater.load(state, &m_prototypes, false);
		
		m_demoWriter.start(getCurrentTimeString());
		m_demoWriter.write(state);
		S::log.add("Went active", {LOG_TAGS::HARD_LOG});
		activeLoop();
	}
}

void Root::quit()
{
	S::log.add("End", {LOG_TAGS::HARD_LOG});
	SDLNet_Quit();
	SDL_Quit();
}

void Root::handleNetworkMessage(std::unique_ptr<NetworkMessage> message)
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
			iMsg->serverId = m_inputIdCounter++;
			iMsg->serverStamp = m_gameUpdater.state.timeStamp;
			m_network.sendToAllPlaying(*iMsg);
			m_demoWriter.write(*iMsg);
			m_gameUpdater.addNewInput(std::move(iMsg));
			break;
		}
		case MESSAGE_TYPE::TYPE_CHECKSUM_MSG:
		{
			if (!S::config.desyncMode)
				break;
				
			auto crcMsg = std2::unique_ptr_cast<ChecksumMessage>(std::move(message));
			auto client = m_network.clientByLogin(crcMsg->login);
			if (client->hasDesynced)
				break;
			auto mismatch = m_gameUpdater.crcs.getMismatch(crcMsg->checksums);
			if (mismatch.has_value())
			{
				client->hasDesynced = true;
				S::log.add("Desync detected for client " + std::to_string(crcMsg->login) + " at stamp " + std::to_string(mismatch.value()), {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
				
				StateRequestMessage srMsg;
				srMsg.states.push_back(mismatch.value() - m_prototypes.variables.fixedStepDuration);
				srMsg.states.push_back(mismatch.value());
				
				if (client->requestedStates.size() > 0)
				{
					S::log.add("Client " + std::to_string(crcMsg->login) + " is already in desync, awaiting requested states", {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
					break;
				}
				
				client->requestedStates = srMsg.states;								
				
				m_network.send(srMsg, crcMsg->login);
			}
			break;
		}
		case MESSAGE_TYPE::TYPE_GAME_STATE_MSG:
		{
			auto msg = std2::unique_ptr_cast<GameStateMessage>(std::move(message));
			
			auto client = m_network.clientByLogin(msg->login);
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
					auto serverState = m_gameUpdater.getSavedStateByStamp(state.timeStamp);
					if (!serverState.has_value())
					{
						S::log.add("Failed to fetch server state to check desync at " + std::to_string(state.timeStamp),{LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
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
			ddm.data = m_demoWriter.getDemo(demoRequest->demoName);
			ddm.inResponseTo = demoRequest->initiator_id;
			BinarySerializer bs;
			ddm.serialize(bs);
			m_network.send(ddm, demoRequest->login);
			break;
		}
		case MESSAGE_TYPE::TYPE_REQUEST_MSG:
		{
			auto genericRequestMsg = std2::unique_ptr_cast<GenericRequestMessage>(std::move(message));
			
			switch (genericRequestMsg->request)
			{
				case REQUEST_TYPE::REQUEST_JOIN_GAME:
				{
					m_network.addPlayer(genericRequestMsg->login);

					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;					
					
					BinarySerializer::copyThroughSerialization(m_gameUpdater.state, gsMsg.states.emplace_back());
										
					m_network.send(gsMsg, genericRequestMsg->login);

					std::unique_ptr<InputPlayerJoinedMessage> pjMsg = std::make_unique<InputPlayerJoinedMessage>();
					pjMsg->serverId = m_inputIdCounter++;
					pjMsg->serverStamp = m_gameUpdater.state.timeStamp;
					pjMsg->login = genericRequestMsg->login;
					//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					m_network.sendToAllPlaying(*pjMsg);
					m_demoWriter.write(*pjMsg);
					m_gameUpdater.addNewInput(std::move(pjMsg));
					break;
				}
				case REQUEST_TYPE::REQUEST_GAME_STATE:
				{
					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					BinarySerializer::copyThroughSerialization(m_gameUpdater.state, gsMsg.states.emplace_back()); 
					m_network.send(gsMsg, genericRequestMsg->login);
					break;
				}
				case REQUEST_TYPE::REQUEST_PING:
				{
					GenericRequestMessage grMsg;
					grMsg.request = REQUEST_TYPE::REQUEST_PONG;
					grMsg.inResponseTo = genericRequestMsg->initiator_id;
					m_network.send(grMsg, genericRequestMsg->login);
					break;
				}
				case REQUEST_TYPE::REQUEST_DEMO_LIST:
				{
					DemoListMessage dlm;
					dlm.list = m_demoWriter.getList();
					dlm.inResponseTo = genericRequestMsg->initiator_id;
					m_network.send(dlm, genericRequestMsg->login);
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

void Root::loadPrototypes()
{
	std::ifstream file("out/prototypes.json");
	nlohmann::json j = nlohmann::json::parse(file);
	file.close();
	m_prototypes.load(j);
}

void Root::loadConfig()
{
	std::ifstream file("out/config.json");
	nlohmann::json j = nlohmann::json::parse(file);
	file.close();
	S::config.load(j);
	S::config.saveStateInterval = -1; //Server never needs to save states. Unless it checks for desyncs
}

std::string Root::getCurrentTimeString()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%S-%M-%H [%d-%m]");
	return oss.str();
};