#include <TextureMode.h>

void TextureMode::init(Renderer* renderer, Prototypes* prototypes)
{
	m_prototypes = prototypes;
	m_textureView.init(renderer, prototypes);
	parse(S::textures);
	sort();
	arrange();
}

void TextureMode::update(bool isActive)
{
	if (isActive)
	{
		m_textureView.render(m_displays);
	}
}

void TextureMode::parse(TextureFolder& folder)
{
	for(auto& texture : folder.textures)
		m_displays.push_back(TextureDisplay{texture});
	for(auto& f : folder.folders)
		parse(*f);
}

void TextureMode::sort()
{
	std::sort(m_displays.begin(), m_displays.end(), [](TextureDisplay& a, TextureDisplay& b){
		return a.texture->rect.h < b.texture->rect.h;
	});
}
	
void TextureMode::arrange()
{
	const int32_t texturePadding = 1;
	const int32_t screenPadding = 1;
	
	int32_t lineHeight = screenPadding;
	int32_t cursorX = screenPadding;
	int32_t cursorY = screenPadding;
	
	for(auto& display : m_displays)
	{
		if (display.texture->rect.w + cursorX > m_prototypes->variables.fieldWidth - screenPadding)
		{
			cursorY = lineHeight + texturePadding;
			cursorX = screenPadding;			
		}
			
		display.location.x = cursorX + display.texture->rect.w / 2;
		display.location.y = cursorY+  + display.texture->rect.h / 2;
		cursorX += display.texture->rect.w + texturePadding;
		lineHeight = std::max(lineHeight, (int32_t)(cursorY + display.texture->rect.h));		
	}
}

	
void TextureMode::onMouseDown(SDL_MouseButtonEvent* event)
{
	for(auto& display : m_displays)
	{		
		if (
			event->x > display.location.x - display.texture->rect.w / 2 &&
			event->x < display.location.x + display.texture->rect.w / 2 &&
			event->y > display.location.y - display.texture->rect.h / 2 &&
			event->y < display.location.y + display.texture->rect.h / 2
		)
		{
			std::cout << display.texture->name << "\n";
		}
	}
}

void TextureMode::onMouseUp(SDL_MouseButtonEvent* event)
{
	
}

void TextureMode::onMouseMove(SDL_MouseMotionEvent* event)
{
	m_textureView.onMouseMove(event);
}
	
void TextureMode::onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	
}

void TextureMode::onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context)
{
	
}