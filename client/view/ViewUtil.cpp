#include <ViewUtil.h>
#include <FMath.h>

namespace S
{	
	TextureNames textures;
	SequenceNames sequences(textures);
}

namespace ViewUtil
{	
	const SDL_Color FAIL_COLOR = colorFromHex(0xff00ff, 0xaa);

	SDL_Color colorFromHex(uint32_t color, uint8_t alpha)
	{
		return SDL_Color{(uint8_t)((color >> 16) & 0xff), (uint8_t)((color >> 8) & 0xff), (uint8_t)(color & 0xff), alpha};
	}
}

bool operator==(SDL_Color& x, SDL_Color& y)
{
	return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
}

SDL_Color FMath::lerp(float x1, SDL_Color& c1, float x2, SDL_Color& c2, float argument)
{
	if (x1 == x2)
		return c1 == c2 ? c1 : ViewUtil::FAIL_COLOR;
	return SDL_Color{
		lerp(x1, c1.r, x2, c2.r, argument),
		lerp(x1, c1.g, x2, c2.g, argument),
		lerp(x1, c1.b, x2, c2.b, argument),
		lerp(x1, c1.a, x2, c2.a, argument)
		};
}