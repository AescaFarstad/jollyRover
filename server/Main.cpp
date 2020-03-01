
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <NetworkPacket.h>
#include <ServerNetwork.h>
#include <GameUpdater.h>
#include <GameStateMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <GenericRequestMessage.h>
#include <chrono>
#include <thread>
#include <Prototypes.h>
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

void handleNetworkMessage(std::unique_ptr<NetworkMessage> message)
{
	switch (message->typeId)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG:
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG:
		{
			InputMessage* t = dynamic_cast<InputMessage*>(message.release());
			//std::unique_ptr<InputMessage> iMsg = network.factory.pointerByType2<std::unique_ptr<NetworkMessage>>(message->typeId, *t);
			std::unique_ptr<InputMessage> iMsg = std::unique_ptr<InputMessage>(t);
			iMsg->serverId = inputIdCounter++;
			iMsg->serverStamp = gameUpdater.state->timeStamp;
			network.sendToAllPlaying(*iMsg);
			gameUpdater.addNewInput(std::move(iMsg));
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
					gsMsg.state = Serializer::copyThroughSerialization(*gameUpdater.state);
					
					S::log.add("Send state, stamp=" + std::to_string(gameUpdater.state->timeStamp) + " players=" + std::to_string(gameUpdater.state->players.size()));
					network.send(gsMsg, genericRequestMsg->login);

					std::unique_ptr<InputPlayerJoinedMessage> pjMsg = std::make_unique<InputPlayerJoinedMessage>();
					pjMsg->serverId = inputIdCounter++;
					pjMsg->serverStamp = gameUpdater.state->timeStamp;
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
					gsMsg.state = Serializer::copyThroughSerialization(*gameUpdater.state); 
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
	S::config.saveStateInterval = -1; //Server never needs to save states
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
		auto state = std::make_unique<GameState>();
		state->timeStamp = ticks;
		lastTicks = ticks;
		gameUpdater.load(std::move(state), &prototypes, false);
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
		[](int32_t login) -> bool {return GameLogic::playerByLogin(gameUpdater.state.get(), login) != nullptr;},
		[](int32_t login) -> bool {return GameLogic::playerByLogin(gameUpdater.state.get(), login) != nullptr;},
		&prototypes.variables
		);	
		
	gameUpdater.load(std::make_unique<GameState>(), &prototypes, false);

	while (!isFinished)
		mainLoop();

	SDLNet_Quit();
	SDL_Quit();

	return 0;
}




