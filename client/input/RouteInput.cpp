#include <RouteInput.h>
#include <GameLogic.h>
#include <FMath.h>
#include <Edge.h>


RouteInput::RouteInput()
{
	m_state = ROUTE_STATE::EMPTY;
	m_isLoaded = false;
}

void RouteInput::load(Prototypes* prototypes)
{
	this->m_prototypes = prototypes;
	m_isLoaded = true;
}

ROUTE_STATE RouteInput::getState()
{
	return m_state;
}

std::vector<Point> RouteInput::getPoints()
{
	std::vector<Point> result;
	result.resize(m_route.size());
	std::transform(m_route.begin(), m_route.end(), result.begin(), [](RoutePoint& rp){ return rp.location; });
	return result;
}

std::vector<RoutePoint>& RouteInput::getRoutePoints()
{
	return m_route;
}

void RouteInput::onMouseDown(SDL_MouseButtonEvent* event)
{
	if (!m_isLoaded)
		return;
		
	if (m_state != ROUTE_STATE::EMPTY)
		reset();
		
	if (event->x < m_prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < m_prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);
		m_route.push_back(RoutePoint{Point(event->x, m_prototypes->variables.fieldHeight), true});
		GameLogic::buildRouteToTarget(touch, m_route, m_prototypes);

		m_state = ROUTE_STATE::ACTIVE;
	}
}

void RouteInput::onMouseUp(SDL_MouseButtonEvent* event)
{
	if (!m_isLoaded)
		return;
	
	if (m_state == ROUTE_STATE::ACTIVE)
	{
		if (m_route.size() < (size_t)m_prototypes->variables.minRouteSteps)
		{
			m_state = ROUTE_STATE::E_TOO_SHORT;
			return;
		}
		
		RoutePoint finish{Point(event->x, m_prototypes->variables.fieldHeight), true}; //Assumed no obstacles next to the field edge
		if (!GameLogic::isRouteAnglePositive(m_route, finish.location, m_prototypes))
		{
			m_state = ROUTE_STATE::E_GOES_UP;
			return;
		}
		
		GameLogic::buildRouteToTarget(finish.location, m_route, m_prototypes);
		m_route.push_back(finish);
		
		if (m_route.size() > (size_t)m_prototypes->variables.maxRouteSteps)
		{
			m_state = ROUTE_STATE::E_TOO_LONG;
			return;
		}
		
		for (RoutePoint& p : m_route)
		{
			if (!p.isValid_)
			{
				m_state = ROUTE_STATE::E_COLLIDES;
				return;
			}
		}
		
		m_state = ROUTE_STATE::VALID;
	}
}

void RouteInput::onMouseMove(SDL_MouseMotionEvent* event)
{
	if (m_state != ROUTE_STATE::ACTIVE || !m_isLoaded)
		return;

	if (event->x < m_prototypes->variables.fieldWidth && event->x > 0 &&
		event->y < m_prototypes->variables.fieldHeight && event->y > 0)
	{
		Point touch(event->x, event->y);

		while (m_route.size() > 1 && !GameLogic::isRouteAnglePositive(m_route, touch, m_prototypes))
		{
			//S::log.add("-point:" + m_route.back().toString(), { LOG_TAGS::INPUT_ });
			m_route.pop_back();
		}

		GameLogic::buildRouteToTarget(touch, m_route, m_prototypes);
	}
}


void RouteInput::reset()
{
	m_route.clear();
	m_state = ROUTE_STATE::EMPTY;
}
