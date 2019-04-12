#pragma once
#include <Point.h>
#include <SDL_gpu.h>
#include <ViewUtil.h>

class Renderer
{
public:
	Renderer() = default;
	
	void init(GPU_Target* screen, GPU_Image* atlas);
	void blit(TextureDef& texture, Point& location, float rotation = 0, float scale = 1);
	GPU_Target* getScreen();
	
private:
	GPU_Target* m_screen;
	GPU_Image* m_atlas;
};
