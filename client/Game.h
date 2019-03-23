#pragma once

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif

#include <Network.h>
#include <GameUpdater.h>
#include <GameView.h>
#include <Keyboard.h>
#include <TaskManager.h>
#include <Prototypes.h>
#include <RouteInput.h>

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
	void loadConfig();

private:
	void handleGenericRequest(GenericRequestMessage* request);
	void addNetworkBindings();
	void handleGameInput(std::unique_ptr<NetworkMessage>);



};