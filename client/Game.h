#pragma once

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL.h>
#endif

#include <Network.h>
#include <GameMode.h>
#include <TextureMode.h>
#include <Keyboard.h>
#include <TaskManager.h>
#include <Prototypes.h>
#include <SDL_gpu.h>

class Game
{
public:
	Game(GPU_Target* screen);
	~Game();

	void load();
	void update();
	void handleEvent(SDL_Event* event);
	void loadPrototypes();
	void loadConfig();

private:

	Network* m_network;
	TaskManager* m_taskManager;
	Prototypes m_prototypes;
	
	std::vector<Mode*> m_modes;
	GameMode m_gameMode;
	Mode* m_activeMode; 

	Keyboard m_keyboard;
	int32_t m_login;
	std::vector<int8_t> m_password;
	
	void handleGenericRequest(GenericRequestMessage* request);
	void addNetworkBindings();
	void handleGameInput(std::unique_ptr<NetworkMessage>);



};