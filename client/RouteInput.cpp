#include <RouteInput.h>
#include <GameLogic.h>
#include <FMath.h>
#include <Edge.h>


RouteInput::RouteInput()
{
	m_isInputActive = false;
	m_isLoaded = false;
}

void RouteInput::load(GameState* state, Prototypes* prototypes)
{
	this->m_state = state;
	this->m_prototypes = prototypes;
	m_isLoaded = true;
}

bool RouteInput::isCompletelyValid()
{
	return m_isCompletelyValid;
}

bool RouteInput::isLoaded()
{
	return m_isLoaded;
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
	if (!m_isLoaded)
		return;
	if (event->x < m_prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < m_prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);
		//S::log.add(touch.toString());
		route.push_back(RoutePoint{Point(event->x, m_prototypes->variables.fieldHeight), true});
		GameLogic::buildRouteToTarget(touch, route, m_prototypes);
		//GameLogic::isRouteAnglePositive(route, touch, prototypes);

		m_isInputActive = true;
	}
}

void RouteInput::onMouseUp(SDL_MouseButtonEvent* event)
{
	if (!m_isLoaded)
		return;

	m_isInputActive = false;
	if (route.size() < (size_t)m_prototypes->variables.minRouteSteps)
	{
		reset();
		return;
	}

	RoutePoint finish{Point(event->x, m_prototypes->variables.fieldHeight), true}; //Assumed no obstacles next to the field edge
	if (!GameLogic::isRouteAnglePositive(route, finish.location, m_prototypes))
	{
		reset();
		return;
	}

	GameLogic::buildRouteToTarget(finish.location, route, m_prototypes);
	route.push_back(finish);

	m_isCompletelyValid = true;

	for (RoutePoint& p : route)
	{
		if (!p.isValid_)
		{
			m_isCompletelyValid = false;
			break;
		}
	}

	if (m_isCompletelyValid)
	{
		S::log.add("Route input is valid!", { LOG_TAGS::INPUT_ });
	}
}

void RouteInput::onMouseMove(SDL_MouseMotionEvent* event)
{
	if (!m_isInputActive || !m_isLoaded)
		return;

	if (event->x < m_prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < m_prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);

		while (route.size() > 1 && !GameLogic::isRouteAnglePositive(route, touch, m_prototypes))
		{
			//S::log.add("-point:" + route.back().toString(), { LOG_TAGS::INPUT_ });
			route.pop_back();
		}

		GameLogic::buildRouteToTarget(touch, route, m_prototypes);
	}
}


void RouteInput::reset()
{
	route.clear();
	m_isInputActive = false;
	m_isCompletelyValid = false;
}
