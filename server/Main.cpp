
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
#include <chrono>
#include <thread>
#include <Prototypes.h>
#undef main

const int MIN_TIME_PER_FRAME = 10;

bool isFinished = false;
int lastTicks = 0;
ServerNetwork network;
std::unique_ptr<NetworkMessage>* messageBuffer;
GameUpdater gameUpdater;
int32_t inputIdCounter = 0;
Prototypes prototypes;


void handleNetworkMessage(std::unique_ptr<NetworkMessage> message)
{
	switch (message->typeId)
	{
		case MessageTypes::TYPE_INPUT_ACTION_MSG:
		case MessageTypes::TYPE_INPUT_LEFT_MSG:
		case MessageTypes::TYPE_INPUT_ROUTE_MSG:
		case MessageTypes::TYPE_INPUT_JOINED_MSG:
		{
			InputMessage* t = dynamic_cast<InputMessage*>(message.release());
			//std::unique_ptr<InputMessage> iMsg = network.factory.pointerByType2<std::unique_ptr<NetworkMessage>>(message->typeId, *t);
			std::unique_ptr<InputMessage> iMsg = std::unique_ptr<InputMessage>(t);
			iMsg->serverId = inputIdCounter++;
			iMsg->serverStamp = gameUpdater.state->timeStamp;
			network.sendToAllPlaying(iMsg.get());
			gameUpdater.addNewInput(std::move(iMsg));
			break;
		}
		case MessageTypes::TYPE_REQUEST_MSG:
		{
			GenericRequestMessage* t = dynamic_cast<GenericRequestMessage*>(message.get());
			std::unique_ptr<GenericRequestMessage> genericRequestMsg = std::make_unique<GenericRequestMessage>(*t);
			switch (genericRequestMsg->request)
			{
				case RequestTypes::REQUEST_JOIN_GAME:
				{
					network.addPlayer(genericRequestMsg->login);

					GameStateMessage gsMsg;
					SerializationStream stream(new StreamGrowerExp(256, 2));
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					gsMsg.state = gameUpdater.state.get();
					network.send(&gsMsg, genericRequestMsg->login);

					std::unique_ptr<InputPlayerJoinedMessage> pjMsg = std::make_unique<InputPlayerJoinedMessage>();
					pjMsg->serverId = inputIdCounter++;
					pjMsg->serverStamp = gameUpdater.state->timeStamp;
					pjMsg->login = genericRequestMsg->login;
					//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					network.sendToAllPlaying(pjMsg.get());
					gameUpdater.addNewInput(std::move(pjMsg));
					break;
				}
				case RequestTypes::REQUEST_GAME_STATE:
				{
					GameStateMessage gsMsg;
					SerializationStream stream(new StreamGrowerExp(256, 2));
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					gsMsg.state = gameUpdater.state.get(); 
					network.send(&gsMsg, genericRequestMsg->login);
					break;
				}
				case RequestTypes::REQUEST_PING:
				{
					GenericRequestMessage grMsg;
					grMsg.request = RequestTypes::REQUEST_PONG;
					grMsg.inResponseTo = genericRequestMsg->initiator_id;
					network.send(&grMsg, genericRequestMsg->login);
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

#include <DEBUG.h>
void loadPrototypes()
{
	//system("dir /b > test.txt");
	/*
	std::ifstream f("test.txt");
	f.seekg(0, std::ios::end);    // go to the end
	int length = f.tellg();           // report location (this is the length)
	f.seekg(0, std::ios::beg);    // go back to the beginning
	char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
	f.read(buffer, length);       // read the whole file into the buffer
	f.close();
	printf("f:\n%s", buffer);

	std::ifstream file("prototypes.json");

	file.seekg(0, std::ios::end);    // go to the end
	length = file.tellg();           // report location (this is the length)
	file.seekg(0, std::ios::beg);    // go back to the beginning
	buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
	file.read(buffer, length);       // read the whole file into the buffer
	file.close();
	printf("file:\n%s", buffer);*/
	std::ifstream file("prototypes.json");
	json j = json::parse(file);
	file.close();
	prototypes.load(j);
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
		else
		{
			//game->handleEvent(&e);
		}
	}


	network.update(messageBuffer);
	int i = 0;
	while (messageBuffer[i] != nullptr)
	{
		handleNetworkMessage(std::move(messageBuffer[i]));
		messageBuffer[i] = nullptr;
		i++;
	}

	int ticks = SDL_GetTicks();
	int delta = ticks - lastTicks;
	if (delta >= MIN_TIME_PER_FRAME)
	{
		lastTicks = ticks;
		gameUpdater.update(ticks);
	}
}


void initLog()
{
	S::log.disableTags({
		//LOG_TAGS::UNIQUE,
		//LOG_TAGS::NET,
		//LOG_TAGS::GAME,

		//LOG_TAGS::NET_MESSAGE,
		Z
	});

	S::log.enableTags({
		LOG_TAGS::UNIQUE,
		LOG_TAGS::NET,
		//LOG_TAGS::GAME,
		LOG_TAGS::ERROR_,

		//LOG_TAGS::NET_MESSAGE,
		Z
	});
}

int main()
{
	printf(".\n");

	SDL_Init(SDL_INIT_EVERYTHING);

	SDLNet_Init();

	initLog();

	network.init();

	loadPrototypes();

	messageBuffer = new std::unique_ptr<NetworkMessage>[SERVER_CONST::messageBufferSize + 1];
	std::unique_ptr<GameState> state = std::make_unique<GameState>();
	gameUpdater.load(std::move(state), &prototypes);

	while (!isFinished)
		mainLoop();

	network.finish();

	SDLNet_Quit();
	SDL_Quit();

	return 0;
}



