#pragma once
#include <GameState.h>
#include <InputMessage.h>
#include <Point.h>
#include <Edge.h>
#include <FMath.h>
#include <GameLogic.h>
#include <Prototypes.h>
#include <vector>
#include <functional>

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif

class RoutePoint;
class RouteInput
{
public:
	RouteInput();
	~RouteInput();

	std::vector<RoutePoint> route;
	bool isInputActive;

	void load(GameState* state, Prototypes* prototypes, std::function<void(std::unique_ptr<InputMessage>)> sendInputFunc);

	void onMouseDown(SDL_MouseButtonEvent* event);
	void onMouseUp(SDL_MouseButtonEvent* event);
	void onMouseMove(SDL_MouseMotionEvent* event);

private:

	bool isLoaded;
	GameState* state;
	Prototypes* prototypes;
	std::function<void(std::unique_ptr<InputMessage>)> sendInputFunc;

	void buildPath(Point &target);
	bool isAnglePositive(Point &target);
	bool testIsValid(Point &from, Point &to);
	void reset();

};

class RoutePoint : public Point
{
public:
	RoutePoint();
	RoutePoint(float x, float y);
	~RoutePoint();

	bool isValid;
};



/*
 down-> route goes to bottom in a straight line, input mode activated

 wrong terrain - highlight in red

 move - depending on the angle - advance or retract

 up-> if valid - send to server

 if invalid - reset

*/