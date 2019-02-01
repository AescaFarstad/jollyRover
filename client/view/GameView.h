#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <RouteInput.h>
#include <memory>
#include <SDL.h>

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

