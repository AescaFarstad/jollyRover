#include <Renderer.h>

void Renderer::init(GPU_Target* screen, GPU_Image* atlas)
{
	m_screen = screen;
	m_atlas = atlas;
	m_blendMode = GPU_BlendPresetEnum::GPU_BLEND_NORMAL;
}

void Renderer::blit(TextureDef& texture, const Point& location, float rotation, float scale, uint8_t alpha)
{
	GPU_SetRGBA(m_atlas, 255, 255, 255, alpha);
	GPU_BlitTransform(m_atlas, &texture.rect, m_screen, location.x, location.y, rotation / M_PI * 180, scale, scale); 
}

void Renderer::blit(TextureDef& texture, const Point& location, float rotation, float scale, SDL_Color& tint)
{
	GPU_SetRGBA(m_atlas, tint.r, tint.g, tint.r, tint.a);
	GPU_BlitTransform(m_atlas, &texture.rect, m_screen, location.x, location.y, rotation / M_PI * 180, scale, scale); 
}

void Renderer::blit(TextureDef& texture, TweenParams& params)
{
	blit(texture, params.location, params.rotation, params.scale, params.tint);
}

void Renderer::blit(GPU_Image* customTexture, GPU_Rect& rect, const Point& location, float rotation, float scale, uint8_t alpha)
{
	GPU_SetRGBA(m_atlas, 255, 255, 255, alpha);
	GPU_BlitTransform(customTexture, &rect, m_screen, location.x, location.y, rotation / M_PI * 180, scale, scale); 
}

void Renderer::setBlendMode(GPU_BlendPresetEnum mode)
{
	m_blendMode = mode;
	GPU_SetBlendMode(m_atlas, mode);
}

GPU_BlendPresetEnum Renderer::getBlendMode()
{
	return m_blendMode;
}
	

GPU_Target* Renderer::getScreen()
{
	return m_screen;
}