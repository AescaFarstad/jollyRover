#include <PhantomRoute.h>
#include <std2.h>

PhantomRoute::PhantomRoute(std::vector<RoutePoint> route, int32_t stamp) : stamp(stamp), route(route){}

PhantomRoute::PhantomRoute() : stamp(-99999){}
