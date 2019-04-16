#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <RouteInput.h>
#include <memory>
#include <SDL_gpu.h>
#include <Renderer.h>
#include <ProjectileExplosionView.h>
#include <ViewController.h>

#ifdef __EMSCRIPTEN__
	#include <SDL_image.h>
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
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
	Prototypes* m_prototypes;
	bool m_isInitialized;
	GameState* m_state;
	RouteInput* m_routeInput;
	Renderer m_renderer;
	GPU_Image* m_image;	
	ViewController<ProjectileExplosionView> m_projectileExplosions;	

	void init();
	void setColor(uint32_t color);
	void drawPlayers();
	void drawObstacles();
	void drawInput();
	void drawCars();
	void drawCreeps();
	void drawProjectiles();
	void drawFormations();
	void drawProjectileExplosion();
	void drawUnitExplosion();
	
};

