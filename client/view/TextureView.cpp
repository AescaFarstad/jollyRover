#include <TextureView.h>

void TextureView::init(Renderer* renderer, Prototypes* prototypes)
{
	m_screen = renderer->getScreen();
	m_renderer = renderer;
	m_prototypes = prototypes;
	
}

void TextureView::render(std::vector<TextureDisplay>& displays)
{
	SDL_Color white = colorFromHex(0xffffff);
	GPU_RectangleFilled(m_screen, 0, 0, m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight, white);
	
	for(auto& display : displays)
	{
		m_renderer->blit(*display.texture, display.location);
		
		if (
			m_mouseLocation.x > display.location.x - display.texture->rect.w / 2 &&
			m_mouseLocation.x < display.location.x + display.texture->rect.w / 2 &&
			m_mouseLocation.y > display.location.y - display.texture->rect.h / 2 &&
			m_mouseLocation.y < display.location.y + display.texture->rect.h / 2
		)
		{
			SDL_Color color = colorFromHex(0x0000ff, 0x77);
			GPU_Rectangle(
					m_screen,
					display.location.x - display.texture->rect.w / 2,
					display.location.y - display.texture->rect.h / 2,
					display.location.x + display.texture->rect.w / 2,
					display.location.y + display.texture->rect.h / 2,
					color
				);
		}
	}
	
}

void TextureView::onMouseMove(SDL_MouseMotionEvent* event)
{
	m_mouseLocation.x = event->x;
	m_mouseLocation.y = event->y;	
}