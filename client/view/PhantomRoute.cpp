#include <PhantomRoute.h>
#include <std2.h>

PhantomRoute::PhantomRoute(std::vector<RoutePoint> route, int32_t stamp) : route(route), stamp(stamp){}

PhantomRoute::PhantomRoute() : stamp(-99999){}
