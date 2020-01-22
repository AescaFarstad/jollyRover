#pragma once
#include <KeyboardInputContext.h>
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
	virtual ~Mode() = default;

	virtual void update(bool isActive) = 0;

	virtual void onMouseDown(const SDL_MouseButtonEvent& event) = 0;
	virtual void onMouseUp(const SDL_MouseButtonEvent& event) = 0;
	virtual void onMouseMove(const SDL_MouseMotionEvent& event) = 0;
	
	
	virtual	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context) = 0;
	virtual	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context) = 0;

private:

};