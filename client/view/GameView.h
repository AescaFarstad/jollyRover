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
#else 
	#include <SDL2/SDL_image.h>
#endif



class GameView
{
public:
	GameView(SDL_Window* window, SDL_Renderer* renderer, Prototypes* prototypes);
	~GameView();

	void render(GameState* state, RouteInput* routeInput);
	
private:

	SDL_Window* m_window;
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
	
};

namespace S
{	
	extern TextureNames textures;
}

