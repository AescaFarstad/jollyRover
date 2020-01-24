
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
	bool hasClients = false;
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
	int32_t i = 0;
	while (messageBuffer[i] != nullptr)
	{
		handleNetworkMessage(std::move(messageBuffer[i]));
		messageBuffer[i] = nullptr;
		i++;
	}
	
	bool newHasClients = network.hasClients();
	if (hasClients && !newHasClients)
		gameUpdater.load(std::make_unique<GameState>(), &prototypes, false);
	hasClients = newHasClients;

	int32_t ticks = SDL_GetTicks();
	int32_t delta = ticks - lastTicks;
	if (delta >= MIN_TIME_PER_FRAME)
	{
		lastTicks = ticks;
		if (hasClients)
			gameUpdater.update(ticks);
	}
}

int main()
{
	printf(".\n");

	SDL_Init(SDL_INIT_EVERYTHING);
	SDLNet_Init();

	loadPrototypes();
	loadConfig();
	
	network.init();

	gameUpdater.load(std::make_unique<GameState>(), &prototypes, false);

	while (!isFinished)
		mainLoop();

	SDLNet_Quit();
	SDL_Quit();

	return 0;
}




