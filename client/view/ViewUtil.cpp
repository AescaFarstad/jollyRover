#include <ViewUtil.h>

SDL_Color colorFromHex(uint32_t color, uint32_t alpha)
{
	return SDL_Color{(color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, alpha};
}