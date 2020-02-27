#include <ReconnectMode.h>

void ReconnectMode::init(Renderer* renderer, Prototypes* prototypes)
{
	m_prototypes = prototypes;
	m_renderer = renderer;
}

void ReconnectMode::update(bool isActive)
{
	if (isActive)
	{
		auto& vars = m_prototypes->variables;
		auto screen = m_renderer->getScreen();
		
		m_ticksPassed++;
		GPU_RectangleFilled(screen, 0, 0, vars.fieldWidth, vars.fieldHeight, ViewUtil::colorFromHex(0xffdd77, 0xff));
		S::fonts.fontAmaticBoldBig.draw(screen, vars.fieldWidth / 2, vars.fieldHeight / 2 - 20, NFont::AlignEnum::CENTER, "Lost connection to the server   :(");
		S::fonts.fontAmaticBoldBig.draw(screen, vars.fieldWidth / 2, vars.fieldHeight / 2 + 20, NFont::AlignEnum::CENTER, "Attempting to reconnect ");
		
		if (m_ticksPassed% 200 == 0 && !S::network->isConnected())
		{
			S::log.add("Reconnect attempt", {LOG_TAGS::NET});
			S::network->connect();			
		}
			
	}
}

	
void ReconnectMode::onMouseDown(const SDL_MouseButtonEvent& event)
{
	
}

void ReconnectMode::onMouseUp(const SDL_MouseButtonEvent& event)
{
	
}

void ReconnectMode::onMouseMove(const SDL_MouseMotionEvent& event)
{
	
}
	
void ReconnectMode::onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	
}

void ReconnectMode::onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	
}