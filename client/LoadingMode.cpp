#include <LoadingMode.h>

void LoadingMode::init(Renderer* renderer, Prototypes* prototypes, std::function<void()> onGoOffline)
{
	m_prototypes = prototypes;
	m_renderer = renderer;
	m_onGoOffline = onGoOffline;
}

void LoadingMode::update(bool isActive)
{
	auto& vars = m_prototypes->variables;
	auto screen = m_renderer->getScreen();
	if (isActive)
	{
		m_ticksPassed++;
		GPU_RectangleFilled(screen, 0, 0, vars.fieldWidth, vars.fieldHeight, ViewUtil::colorFromHex(0xffff99, 0xff));
		S::fonts.fontAmaticBoldBig.draw(screen, vars.fieldWidth / 2, vars.fieldHeight / 2, NFont::AlignEnum::CENTER, "Unable to connect   :(");
		
		if (m_ticksPassed > SHOW_OFFLINE_PROMPT_AT)
			S::fonts.fontAmaticBoldBig.draw(screen, vars.fieldWidth / 2, vars.fieldHeight  - 50, NFont::AlignEnum::CENTER, "Press Enter to play offline");
			
	}
}

	
void LoadingMode::onMouseDown(const SDL_MouseButtonEvent& event)
{
	
}

void LoadingMode::onMouseUp(const SDL_MouseButtonEvent& event)
{
	
}

void LoadingMode::onMouseMove(const SDL_MouseMotionEvent& event)
{
	
}
	
void LoadingMode::onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	//if (m_ticksPassed > SHOW_OFFLINE_PROMPT_AT)
		if (scancode == SDL_Scancode::SDL_SCANCODE_RETURN)
			m_onGoOffline();
}

void LoadingMode::onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	
}