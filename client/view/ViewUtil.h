#pragma once

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL_pixels.h>
#endif

///0xrrggbb 0xaa
SDL_Color colorFromHex(uint32_t color, uint32_t alpha = 0xff);