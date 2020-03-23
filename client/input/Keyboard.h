#pragma once
//#include <stdlib.h>

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL.h>
#endif

class Keyboard
{
public:
	Keyboard();
	bool isDown[SDL_NUM_SCANCODES];
};
