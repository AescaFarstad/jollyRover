#include <RouteInput.h>
#include <GameLogic.h>
#include <FMath.h>
#include <Edge.h>


RouteInput::RouteInput()
{
	isInputActive = false;
	isLoaded = false;
}

void RouteInput::load(GameState* state, Prototypes* prototypes)
{
	this->state = state;
	this->prototypes = prototypes;
	isLoaded = true;
}

std::vector<Point> RouteInput::claimRoute()
{
	std::vector<Point> routeAsPoints;
	for (RoutePoint& rp : route)
	{
		routeAsPoints.push_back(rp.location);
	}
	reset();
	return routeAsPoints;
}


void RouteInput::onMouseDown(SDL_MouseButtonEvent* event)
{
	if (!isLoaded)
		return;
	if (event->x < prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);
		//S::log.add(touch.toString());
		route.push_back(RoutePoint{Point(event->x, prototypes->variables.fieldHeight), true});
		GameLogic::buildRouteToTarget(touch, route, prototypes);
		//GameLogic::isRouteAnglePositive(route, touch, prototypes);

		isInputActive = true;
	}
}

void RouteInput::onMouseUp(SDL_MouseButtonEvent* event)
{
	if (!isLoaded)
		return;

	isInputActive = false;
	if (route.size() < (size_t)prototypes->variables.minRouteSteps)
	{
		reset();
		return;
	}

	RoutePoint finish{Point(event->x, prototypes->variables.fieldHeight), true}; //Assumed no obstacles next to the field edge
	if (!GameLogic::isRouteAnglePositive(route, finish.location, prototypes))
	{
		reset();
		return;
	}

	GameLogic::buildRouteToTarget(finish.location, route, prototypes);
	route.push_back(finish);

	isCompletelyValid = true;

	for (RoutePoint& p : route)
	{
		if (!p.isValid_)
		{
			isCompletelyValid = false;
			break;
		}
	}

	if (isCompletelyValid)
	{
		S::log.add("Route input is valid!", { LOG_TAGS::INPUT_ });
	}
}

void RouteInput::onMouseMove(SDL_MouseMotionEvent* event)
{
	if (!isInputActive || !isLoaded)
		return;

	if (event->x < prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);

		while (route.size() > 1 && !GameLogic::isRouteAnglePositive(route, touch, prototypes))
		{
			//S::log.add("-point:" + route.back().toString(), { LOG_TAGS::INPUT_ });
			route.pop_back();
		}

		GameLogic::buildRouteToTarget(touch, route, prototypes);
	}
}


void RouteInput::reset()
{
	route.clear();
	isInputActive = false;
	isCompletelyValid = false;
}
