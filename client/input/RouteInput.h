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
enum class ROUTE_STATE
{
	EMPTY,
	ACTIVE,
	VALID,
	E_TOO_SHORT,
	E_TOO_LONG,
	E_COLLIDES,
	E_GOES_UP,
	E_RANDOM_CLICK
};

class RouteInput
{
public:
	RouteInput();
	~RouteInput() = default;


	void load(Prototypes* m_prototypes);
	
	ROUTE_STATE getState();
	std::vector<Point> getPoints();
	std::vector<RoutePoint>& getRoutePoints();
	
	void reset();	

	void onMouseDown(const SDL_MouseButtonEvent& event);
	void onMouseUp(const SDL_MouseButtonEvent& event);
	void onMouseMove(const SDL_MouseMotionEvent& event);
	

private:

	bool m_isLoaded;	
	ROUTE_STATE m_state;
	int32_t m_autoDrawnPoints;
	
	Prototypes* m_prototypes;
	std::vector<RoutePoint> m_route;
	

};