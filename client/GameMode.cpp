#include <GameMode.h>

GameMode::GameMode()
{
	m_isLoaded = false;
}

void GameMode::init(Renderer* renderer, Prototypes* prototypes, Network* network)
{
	m_prototypes = prototypes;
	m_gameView.init(renderer, prototypes);
	m_network = network;
}

void GameMode::loadGame(std::unique_ptr<GameState> state, int64_t clientToServerDelta, int32_t login)
{
	m_gameUpdater.load(std::move(state), m_prototypes, true);
	m_clientToServerDelta = clientToServerDelta;
	m_routeInput.load(m_gameUpdater.state.get(), m_prototypes);
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
		if (m_routeInput.isCompletelyValid())
		{
			auto route = m_routeInput.claimRoute();
			
			auto thisPlayer = GameLogic::playerByLogin(m_gameUpdater.state.get(), m_login);
			if (thisPlayer->refuelLeft >0 || thisPlayer->repairsLeft > 0 || thisPlayer->activeCars.size() > 0)
			{
				auto loc = route.back();
				loc.y -= 60;
				m_gameView.addMessage("Not ready yet!", loc);
			}
			else
			{
				InputRouteMessage nim(route);
				m_network->send(&nim);				
			}
			
		}
		m_gameView.render(m_gameUpdater.state.get(), &m_routeInput);
	}
	else
	{
		int64_t laggingTime = SDL_GetTicks() + m_clientToServerDelta - 1000;
		laggingTime = std::max((int64_t)0, laggingTime);
		m_gameUpdater.update(laggingTime);
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
	
void GameMode::onMouseDown(SDL_MouseButtonEvent* event)
{
	if (!m_isLoaded)
		return;		
	m_routeInput.onMouseDown(event);
	//m_gameView.addMessage("test message. please ignore.", Point(event->x, event->y));
	/*if (event->button == 0x1)
		std::cout<<("\t\t{ \"x\":" + std::to_string(event->x) + ", \"y\":" + std::to_string(event->y) + " },")<<"\n";
	if (event->button == 0x3)
		std::cout<<("\t]\n\t},\n\t{\n\t\t\"vertices\":[")<<"\n";*/
}

void GameMode::onMouseUp(SDL_MouseButtonEvent* event)
{
	if (!m_isLoaded)
		return;
	m_routeInput.onMouseUp(event);	
}

void GameMode::onMouseMove(SDL_MouseMotionEvent* event)
{
	if (!m_isLoaded)
		return;
	m_routeInput.onMouseMove(event);
	m_gameView.onMouseMove(event);
}
	
void GameMode::onKeyDown(SDL_Scancode scancode, Keyboard& keyboard)
{
	if (!m_isLoaded)
		return;
	m_keyboardInput.onKeyDown(scancode, keyboard, *m_network, m_gameUpdater);
}

void GameMode::onKeyUp(SDL_Scancode scancode, Keyboard& keyboard)
{
	if (!m_isLoaded)
		return;
	m_keyboardInput.onKeyUp(scancode, keyboard, *m_network, m_gameUpdater);	
}