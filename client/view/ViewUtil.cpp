#include <ViewUtil.h>

namespace S
{	
	TextureNames textures;
}

SDL_Color colorFromHex(uint32_t color, uint8_t alpha)
{
	return SDL_Color{(uint8_t)((color >> 16) & 0xff), (uint8_t)((color >> 8) & 0xff), (uint8_t)(color & 0xff), alpha};
}
