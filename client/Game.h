#pragma once

//#define WINDOWS 1
#define LINUX 1

#ifdef __EMSCRIPTEN__
#include <SDL.h>
#else 
#include <SDL2/SDL.h>
#endif

#include <Network.h>
#include <GameUpdater.h>
#include <GameState.h>
#include <GameView.h>
#include <Keyboard.h>
#include <LoopBackNetwork.h>
#include <NetworkMessageFactory.h>
#include <SerializationStream.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <TaskManager.h>
#include <ConsequtiveTask.h>
#include <string>
#include <istream>
#include <sstream>
#include <Prototypes.h>
#include <RouteInput.h>
#include <GameInputBinding.h>
#include <AnonymousBinding.h>
#include <json.hpp>
#include <fstream>

class Game
{
public:
	Game(SDL_Window* window, SDL_Renderer* renderer);
	~Game();
	Network* network;
	TaskManager* taskManager;
	GameUpdater gameUpdater;
	GameView* gameView;
	RouteInput* routeInput;
	Prototypes prototypes;

	Keyboard keyboard;
	int32_t login;
	std::vector<int8_t> password;
	int64_t clientToServerDelta;

	void update();
	void handleEvent(SDL_Event* event);
	void loadPrototypes();

private:
	void handleKeyDown(KEYBOARD_ACTIONS code);
	void handleKeyUp(KEYBOARD_ACTIONS code);
	void handleGenericRequest(GenericRequestMessage* request);
	void addNetworkBindings();
	std::vector<AnonymousBinding> bindings;



	std::function<void(std::unique_ptr<NetworkMessage>)> handleGameInput;
	std::function<void(std::unique_ptr<NetworkMessage>)> handleGameState;

};