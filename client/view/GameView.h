#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <RouteInput.h>
#include <memory>
#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif

class GameView
{
public:
	GameView(SDL_Window* window, SDL_Renderer* renderer, Prototypes* prototypes);
	~GameView();

	SDL_Window* window;
	SDL_Renderer* renderer; 
	Prototypes* prototypes;

	void render(GameState* state, RouteInput* routeInput);
private:
	bool isInitialized;
	GameState* state;
	RouteInput* routeInput;

	void init();
	void setColor(uint32_t color);
	void drawPlayers();
	void drawObstacles();
	void drawInput();
	void drawCars();
};

