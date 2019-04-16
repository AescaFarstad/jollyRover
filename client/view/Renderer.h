#pragma once
#include <Point.h>
#include <SDL_gpu.h>
#include <ViewUtil.h>

class Renderer
{
public:
	Renderer() = default;
	
	void init(GPU_Target* screen, GPU_Image* atlas);
	void blit(TextureDef& texture, Point& location, float rotation = 0, float scale = 1, uint8_t alpha = 0xff);
	void blit(TextureDef& texture, Point& location, float rotation, float scale, SDL_Color& tint);
	void blit(TextureDef& texture, TweenParams& params);
	GPU_Target* getScreen();
	
private:
	GPU_Target* m_screen;
	GPU_Image* m_atlas;
};
