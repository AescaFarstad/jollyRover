#pragma once
#include <string>

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#elif LINUX
	#include <SDL2/SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

class TextureDef 
{	
public:	
	std::string name;
	SDL_Rect rect;	
};


class TextureNames 
{
public:	
	TextureNames();

	//42 x 40
	TextureDef tankBody_sand_outline;
	
	//12 x 26s
	TextureDef tankSand_barrel1;
	
};