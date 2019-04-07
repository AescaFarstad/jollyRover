#pragma once

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
	#include <SDL_gpu.h>
#else 
	#include <SDL2/SDL.h>
	#include <SDL_gpu.h>
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
	Game(GPU_Target* screen);
	~Game();

	void update();
	void handleEvent(SDL_Event* event);
	void loadPrototypes();
	void loadConfig();

private:

	Network* m_network;
	TaskManager* m_taskManager;
	GameUpdater m_gameUpdater;
	GameView* m_gameView;
	RouteInput* m_routeInput;
	Prototypes m_prototypes;

	Keyboard m_keyboard;
	int32_t m_login;
	std::vector<int8_t> m_password;
	int64_t m_clientToServerDelta;
	
	void handleGenericRequest(GenericRequestMessage* request);
	void addNetworkBindings();
	void handleGameInput(std::unique_ptr<NetworkMessage>);



};