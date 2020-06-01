#pragma once

#include <string>
#include <GameState.h>
#include <SDL_gpu.h>


class PhantomRoute
{
public:
	static constexpr int32_t LIFETIME = 2000;
	static constexpr float FADE_POINT = 0.65;
	
	PhantomRoute();
	PhantomRoute(std::vector<RoutePoint> route, int32_t stamp);
	
	int32_t stamp;
	std::vector<RoutePoint> route;
	
private:
	

};