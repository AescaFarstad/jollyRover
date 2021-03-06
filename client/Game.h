#pragma once

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL.h>
#endif

#include <GameMode.h>
#include <DemoMode.h>
#include <Network.h>
#include <TextureMode.h>
#include <LoadingMode.h>
#include <ReconnectMode.h>
#include <Keyboard.h>
#include <TaskManager.h>
#include <ConsecutiveTask.h>
#include <Prototypes.h>
#include <SDL_gpu.h>

class Game
{
public:
	Game();
	~Game();

	void init(GPU_Target* screen);
	void start();
	void update();
	void handleEvent(const SDL_Event& event);
	void loadPrototypes();
	void loadConfig();

private:

	TaskManager m_taskManager;
	Prototypes m_prototypes;
	
	GPU_Image* m_atlas;	
	Renderer m_renderer;
	SDL_Window* m_window;
	ConsecutiveTask m_loadGameTask;
	
	std::vector<Mode*> m_modes;
	GameMode m_gameMode;
	DemoMode m_demoMode;
	TextureMode m_textureMode;
	LoadingMode m_loadingMode;
	ReconnectMode m_reconnectMode;
	size_t m_activeMode; 

	KeyboardInputContext m_keyboardContext;
	int32_t m_login;
	std::vector<int8_t> m_password;
	
	void handleGenericRequest(GenericRequestMessage* request);
	void addNetworkBindings();
	void handleGameInput(std::unique_ptr<NetworkMessage>);
	bool handleGlobalKey(SDL_Scancode scancode);
	void runBenchmark();
	void startGame();
	void goOffline();
	



};