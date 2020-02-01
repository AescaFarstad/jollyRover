#include <GameMode.h>

GameMode::GameMode()
{
	m_isLoaded = false;
	m_lastAutodrawNotificationStamp = 0;
	m_numRandomClicks = 0;
}

void GameMode::init(Renderer* renderer, Prototypes* prototypes)
{
	m_prototypes = prototypes;
	m_gameView.init(renderer, prototypes);
}

void GameMode::loadGame(std::unique_ptr<GameState> state, int64_t clientToServerDelta, int32_t login)
{
	m_gameUpdater.load(std::move(state), m_prototypes, true);
	m_clientToServerDelta = clientToServerDelta;
	m_routeInput.load(m_prototypes);
	m_gameView.setLogin(login);
	m_login = login;
	m_isLoaded = true;
}

void GameMode::update(bool isActive)
{
	if (!m_isLoaded)
		return;
		
	if (isActive)
	{
		m_gameUpdater.update(SDL_GetTicks() + m_clientToServerDelta);
		handleRouteInput();
		m_gameView.render(m_gameUpdater.state.get(), &m_routeInput);
	}
	else
	{
		int64_t laggingTime = SDL_GetTicks() + m_clientToServerDelta - 1000;
		laggingTime = std::max((int64_t)0, laggingTime);
		m_gameUpdater.update(laggingTime);
	}
}

Point GameMode::normalizeMessageLocation(Point location)
{
	return Point(
		std::max(200, std::min((int32_t)location.x, m_prototypes->variables.fieldWidth - 200)),
		std::max((int32_t)location.y, 150)
	);
}

void GameMode::handleRouteInput()
{
	switch (m_routeInput.getState())
	{
		case ROUTE_STATE::E_COLLIDES:
		{
			
			for(auto& p : m_routeInput.getRoutePoints())
			{
				if (!p.isValid_)
				{
					m_gameView.addMessage("The route collides with an obstacle!", normalizeMessageLocation(p.location), NFont::AlignEnum::CENTER);
					break;
				}
			}
			m_routeInput.reset();
			break;
		}
		case ROUTE_STATE::E_RANDOM_CLICK:
		{
			auto thisPlayer = GameLogic::playerByLogin(m_gameUpdater.state.get(), m_login);
			if (thisPlayer->refuelLeft >0 || thisPlayer->repairsLeft > 0 || thisPlayer->activeCars.size() > 0)
			{
				auto loc = normalizeMessageLocation(m_routeInput.getRoutePoints().back().location);
				loc.y -= 60;
				m_gameView.addMessage("Not ready yet!", loc, NFont::AlignEnum::CENTER);
			}
			else
			{
				//ignores the first click, shows notification no more than once evry few seconds.
				m_numRandomClicks++;
				if (m_numRandomClicks != 1)
				{
					auto ticks = SDL_GetTicks();
					if (m_lastAutodrawNotificationStamp + 5000 < ticks)
					{
						Point loc = normalizeMessageLocation(m_routeInput.getRoutePoints().back().location);
						m_gameView.addMessage("Hold Left Mouse Button and draw!", loc, NFont::AlignEnum::CENTER);
						m_lastAutodrawNotificationStamp = ticks;
					}
				}
			}
			
			
			m_routeInput.reset();
			break;
		}
		case ROUTE_STATE::E_GOES_UP:
		{
			Point loc = normalizeMessageLocation(m_routeInput.getRoutePoints().back().location);
			m_gameView.addMessage("The route must end at the bottom edge!", loc, NFont::AlignEnum::CENTER);
			m_routeInput.reset();
			break;
		}		
		case ROUTE_STATE::E_TOO_SHORT:
		{
			Point loc(m_prototypes->variables.fieldWidth / 2, m_prototypes->variables.fieldHeight / 2);
			m_gameView.addMessage("The route is too short!", loc, NFont::AlignEnum::CENTER);
			m_routeInput.reset();
			break;
		}				
		case ROUTE_STATE::E_TOO_LONG:
		{
			Point loc(m_prototypes->variables.fieldWidth / 2, m_prototypes->variables.fieldHeight / 2);
			m_gameView.addMessage("The route is too long!", loc, NFont::AlignEnum::CENTER);
			m_routeInput.reset();
			break;
		}
		case ROUTE_STATE::VALID:
		{
			auto thisPlayer = GameLogic::playerByLogin(m_gameUpdater.state.get(), m_login);
			if (thisPlayer->refuelLeft >0 || thisPlayer->repairsLeft > 0 || thisPlayer->activeCars.size() > 0)
			{
				auto loc = normalizeMessageLocation(m_routeInput.getRoutePoints().back().location);
				loc.y -= 60;
				m_gameView.addMessage("Not ready yet!", loc, NFont::AlignEnum::CENTER);
				m_routeInput.reset();
			}
			else
			{
				InputRouteMessage nim;
				nim.route = m_routeInput.getPoints();
				S::network->send(nim);
				m_routeInput.reset();
			}
			break;
		}
		
		default:
			break;
	}
}
		

GameUpdater* GameMode::getGameUpdater()
{
	return &m_gameUpdater;
}

void GameMode::addNewInput(std::unique_ptr<InputMessage> input)
{
	m_gameUpdater.addNewInput(std::move(input));
}
	
void GameMode::onMouseDown(const SDL_MouseButtonEvent& event)
{
	if (!m_isLoaded)
		return;		
	m_routeInput.onMouseDown(event);
}

void GameMode::onMouseUp(const SDL_MouseButtonEvent& event)
{
	if (!m_isLoaded)
		return;
	m_routeInput.onMouseUp(event);	
}

void GameMode::onMouseMove(const SDL_MouseMotionEvent& event)
{
	if (!m_isLoaded)
		return;
	m_routeInput.onMouseMove(event);
	m_gameView.onMouseMove(event);
}
	
void GameMode::onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	if (!m_isLoaded)
		return;
	m_keyboardInput.onKeyDown(scancode, context, m_gameUpdater);
}

void GameMode::onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	if (!m_isLoaded)
		return;
	m_keyboardInput.onKeyUp(scancode, context, m_gameUpdater);	
}