#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <RouteInput.h>
#include <ViewController.h>
#include <CreepView.h>
#include <memory>
#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif


#ifdef __EMSCRIPTEN__
	#include <SDL_image.h>
	#include <SDL_gpu.h>
#else 
	#include <SDL2/SDL_image.h>
	#include <SDL_gpu.h>
#endif



class GameView
{
public:
	GameView(GPU_Target* screen, Prototypes* prototypes);
	~GameView();

	void render(GameState* state, RouteInput* routeInput);
	
private:

	SDL_Window* m_window;
	GPU_Target* m_screen; 
	SDL_Renderer* m_renderer; 
	Prototypes* m_prototypes;
	bool m_isInitialized;
	GameState* m_state;
	RouteInput* m_routeInput;
	
	ViewController<CreepState, CreepView> m_creepViews; 

	void init();
	void setColor(uint32_t color);
	void drawPlayers();
	void drawObstacles();
	void drawInput();
	void drawCars();
	void drawCreeps();
	void drawProjectiles();
	void drawFormations();
	void drawGPU_Test();
	
};

