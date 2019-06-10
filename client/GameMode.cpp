#include <GameMode.h>

GameMode::GameMode()
{
	m_isLoaded = false;
}

void GameMode::init(GPU_Target* screen, Prototypes* prototypes, Network* network)
{
	m_prototypes = prototypes;
	m_gameView.init(screen, prototypes);
	m_network = network;
}

void GameMode::loadGame(std::unique_ptr<GameState> state, int64_t clientToServerDelta)
{
	m_gameUpdater.load(std::move(state), m_prototypes, true);
	m_clientToServerDelta = clientToServerDelta;
	m_routeInput.load(m_gameUpdater.state.get(), m_prototypes);
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
			InputRouteMessage nim(route);
			m_network->send(&nim);
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
	m_routeInput.onMouseDown(event);
}

void GameMode::onMouseUp(SDL_MouseButtonEvent* event)
{
	m_routeInput.onMouseUp(event);	
}

void GameMode::onMouseMove(SDL_MouseMotionEvent* event)
{
	m_routeInput.onMouseMove(event);
	m_gameView.onMouseMove(event);
}
	
void GameMode::handleKeyDown(SDL_Scancode scancode, Keyboard& keyboard)
{
	m_keyboardInput.handleKeyDown(scancode, keyboard, *m_network, m_gameUpdater);
}

void GameMode::handleKeyUp(SDL_Scancode scancode, Keyboard& keyboard)
{
	m_keyboardInput.handleKeyUp(scancode, keyboard, *m_network, m_gameUpdater);	
}