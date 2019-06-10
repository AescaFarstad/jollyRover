#pragma once
#include <Keyboard.h>
#ifdef __EMSCRIPTEN__
	#include <SDL_scancode.h>
	#include <SDL.h>
#else 
	#include <SDL2/SDL_scancode.h>
	#include <SDL2/SDL.h>
#endif

class Mode
{
public:
	Mode() = default;
	virtual ~Mode() = default;

	virtual void update(bool isActive) = 0;

	virtual void onMouseDown(SDL_MouseButtonEvent* event) = 0;
	virtual void onMouseUp(SDL_MouseButtonEvent* event) = 0;
	virtual void onMouseMove(SDL_MouseMotionEvent* event) = 0;
	
	
	virtual	void handleKeyDown(SDL_Scancode scancode, Keyboard& keyboard) = 0;
	virtual	void handleKeyUp(SDL_Scancode scancode, Keyboard& keyboard) = 0;

private:

};