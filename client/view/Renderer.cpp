#include <Renderer.h>

void Renderer::init(GPU_Target* screen, GPU_Image* atlas)
{
	m_screen = screen;
	m_atlas = atlas;
}

void Renderer::blit(TextureDef& texture, Point& location, float rotation, float scale, uint8_t alpha)
{
	GPU_SetRGBA(m_atlas, 255, 255, 255, alpha);
	GPU_BlitTransform(m_atlas, &texture.rect, m_screen, location.x, location.y, rotation / M_PI * 180, scale, scale); 
}

void Renderer::blit(TextureDef& texture, Point& location, float rotation, float scale, SDL_Color& tint)
{
	GPU_SetRGBA(m_atlas, tint.r, tint.g, tint.r, tint.a);
	GPU_BlitTransform(m_atlas, &texture.rect, m_screen, location.x, location.y, rotation / M_PI * 180, scale, scale); 
}

void Renderer::blit(TextureDef& texture, TweenParams& params)
{
	blit(texture, params.location, params.rotation, params.scale, params.tint);
}
	

GPU_Target* Renderer::getScreen()
{
	return m_screen;
}