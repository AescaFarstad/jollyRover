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
	
	void drawArrow(GPU_Target* target, const Point& from, const Point& to, const uint32_t color)
	{
		SDL_Color acolor = ViewUtil::colorFromHex(color, 0xff);
		GPU_Line(target, from.x, from.y, to.x, to.y, acolor);
		
		Point vec = to - from;
		
		float angle = M_PI / 5;
		float headSize = FMath::lerp(0.f, 0.f, 100.f, 8.f, vec.getLength());
		headSize = std::min(headSize, 10.f);
		headSize++;
		
		vec = vec.rotate(angle);
		vec.scaleTo(headSize);
		
		Point branch = to - vec;
		
		GPU_Line(target, to.x, to.y, branch.x, branch.y, acolor);
		
		vec = vec.rotate(-angle*2);
		branch = to - vec;
		
		GPU_Line(target, to.x, to.y, branch.x, branch.y, acolor);
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