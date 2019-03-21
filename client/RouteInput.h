#pragma once
#include <GameState.h>
#include <Point.h>
#include <Prototypes.h>
#include <vector>

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif

class RouteInput
{
public:
	RouteInput();
	~RouteInput() = default;

	std::vector<RoutePoint> route;
	bool isInputActive;
	bool isLoaded;
	bool isCompletelyValid;

	void load(GameState* state, Prototypes* prototypes);
	std::vector<Point> claimRoute();

	void onMouseDown(SDL_MouseButtonEvent* event);
	void onMouseUp(SDL_MouseButtonEvent* event);
	void onMouseMove(SDL_MouseMotionEvent* event);

private:

	GameState* state;
	Prototypes* prototypes;
	
	void reset();

};



/*
 down-> route goes to the bottom in a straight line, input mode activated

 wrong terrain - highlight in red

 move - depends on the angle - advance or retract

 up-> if valid - send to server

 if invalid - reset

*/