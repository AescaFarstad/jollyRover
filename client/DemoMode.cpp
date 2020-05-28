#include <Demo.h>
#include <DemoMode.h>
#include <std2.h>
#include <BinarySerializer.h>
#include <NetworkMessageFactory.h>

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
		m_gameView.render(&m_gameUpdater.state, &m_routeInput);
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