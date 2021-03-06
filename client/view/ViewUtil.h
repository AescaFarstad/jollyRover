#pragma once
#include <TextureNames.h>
#include <SequenceNames.h>
#include <Point.h>
#include <NFont.h>

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL_pixels.h>
#endif

class TweenParams
{
public:
	float scale;
	float rotation;
	Point location;
	SDL_Color tint;
};

class Fonts
{
public:	
	void initFonts();
	NFont fontAmaticBold;
	NFont fontAmaticBoldBig;
	NFont fontAmaticRegular;
	NFont fontDebug;
	NFont fontDebug_m;
};

namespace S
{	
	extern TextureNames textures;
	extern SequenceNames sequences;	
	extern Fonts fonts;
}

namespace ViewUtil
{
	extern const SDL_Color FAIL_COLOR;

	///0xrrggbb 0xaa
	SDL_Color colorFromHex(uint32_t color, uint8_t alpha = 0xff);
	void drawArrow(GPU_Target* target, const Point& from, const Point& to, const uint32_t color);
}

bool operator==(SDL_Color& x, SDL_Color& y);

namespace FMath
{	
	SDL_Color lerp(float x1, SDL_Color& c1, float x2, SDL_Color& c2, float argument);
}

