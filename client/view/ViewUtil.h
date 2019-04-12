#pragma once
#include <TextureNames.h>
#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL_pixels.h>
#endif

namespace S
{	
	extern TextureNames textures;
}

///0xrrggbb 0xaa
SDL_Color colorFromHex(uint32_t color, uint8_t alpha = 0xff);

