#include <Renderer.h>

void Renderer::init(GPU_Target* screen, GPU_Image* atlas)
{
	m_screen = screen;
	m_atlas = atlas;
}

void Renderer::blit(TextureDef& texture, Point& location, float rotation, float scale)
{
	GPU_BlitTransform(m_atlas, &texture.rect, m_screen, location.x, location.y, rotation, scale, scale); 
}

GPU_Target* Renderer::getScreen()
{
	return m_screen;
}