#include <Demo.h>
#include <DemoMode.h>
#include <std2.h>
#include <BinarySerializer.h>
#include <NetworkMessageFactory.h>
#include <GameMode.h>

DemoMode::DemoMode()
{
	m_isLoaded = false;
	m_startTime = 0;
}

void DemoMode::init(Renderer* renderer, Prototypes* prototypes)
{
	m_prototypes = prototypes;
	m_gameView.init(renderer, prototypes);
		
}

void DemoMode::loadGame(const std::vector<char>& demoData)
{
	BinarySerializer bs(demoData);
	
	DEMO_EVENT event;
	bs.read(event);
	
	GameState state;
	bs.read(state);
	m_gameUpdater.load(state, m_prototypes, true);
	
	
	while(bs.getDataLeft() > 0)
	{
		bs.read(event);		
		auto msg = NetworkMessageFactory::parse(bs);		
		m_gameUpdater.addNewInput(std2::unique_ptr_cast<InputMessage>(std::move(msg)));		
	}
	
	m_routeInput.load(m_prototypes);
	m_gameView.setLogin(-100);
	m_isLoaded = true;
	m_startTime = SDL_GetTicks();
	m_startStateStamp = state.timeStamp;
}

void DemoMode::update(bool isActive)
{
	if (!m_isLoaded)
		return;
		
	if (isActive)
	{
		m_gameUpdater.update(SDL_GetTicks() - m_startTime + m_startStateStamp);
		visualizeInputs(m_gameUpdater.getLastFrameInputs());
		m_gameView.render(&m_gameUpdater.state, &m_routeInput);
	}
}
void DemoMode::visualizeInputs(std::vector<InputMessage*> inputs)
{
	for(auto i : inputs)
	{
		if (i->typeId == MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG)
		{
			auto rInput = dynamic_cast<InputRouteMessage*>(i);
			std::vector<RoutePoint> routePoints;
			for(auto p : rInput->route)
			{
				RoutePoint newPoint;
				newPoint.location = p;
				routePoints.push_back(newPoint);
			}
			
			if (routePoints.size() < (size_t)m_prototypes->variables.minRouteSteps)
			{
				Point loc(m_prototypes->variables.fieldWidth / 2, m_prototypes->variables.fieldHeight / 2);
				m_gameView.addMessage("The route is too short!", loc, NFont::AlignEnum::CENTER);
				m_gameView.addPhantomRoute(routePoints);
				continue;
			}
			
			RoutePoint finish{Point(routePoints.back().location.x, m_prototypes->variables.fieldHeight + 1), true};
			if (!GameLogic::isRouteAnglePositive(routePoints, finish.location, m_prototypes))
			{
				Point loc = GameMode::normalizeMessageLocation(routePoints.back().location, m_prototypes);
				m_gameView.addMessage("The route must end at the bottom edge!", loc, NFont::AlignEnum::CENTER);
				m_gameView.addPhantomRoute(routePoints);
				continue;
			}
			
			if (routePoints.size() > (size_t)m_prototypes->variables.maxRouteSteps)
			{
				Point loc(m_prototypes->variables.fieldWidth / 2, m_prototypes->variables.fieldHeight / 2);
				m_gameView.addMessage("The route is too long!", loc, NFont::AlignEnum::CENTER);
				m_gameView.addPhantomRoute(routePoints);
				continue;
			}
			
			for (size_t i = 0; i < routePoints.size() - 1; i++)
			{
				if (!GameLogic::testEdgeIsValid(routePoints[i].location, routePoints[i + 1].location, m_prototypes))
				{
					Point loc = GameMode::normalizeMessageLocation(routePoints[i].location, m_prototypes);
					m_gameView.addMessage("The route collides with an obstacle!", loc, NFont::AlignEnum::CENTER);
					m_gameView.addPhantomRoute(routePoints);
					break;
				}
			}
		}
	}
}
	
void DemoMode::onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	if (!m_isLoaded)
		return;
}

void DemoMode::onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	if (!m_isLoaded)
		return;
}

void DemoMode::onMouseDown(const SDL_MouseButtonEvent& event)
{
	if (!m_isLoaded)
		return;	
}

void DemoMode::onMouseUp(const SDL_MouseButtonEvent& event)
{
	if (!m_isLoaded)
		return;
}

void DemoMode::onMouseMove(const SDL_MouseMotionEvent& event)
{
	if (!m_isLoaded)
		return;
}