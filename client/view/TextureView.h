#pragma once
#include <memory>
#include <SDL_gpu.h>
#include <Renderer.h>
#include <Prototypes.h>

#ifdef __EMSCRIPTEN__
	#include <SDL_image.h>
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

class TextureDisplay
{
public:
	TextureDef* texture;
	Point location;
};

class TextureView
{
public:
	TextureView() = default;

	void init(Renderer* renderer, Prototypes* prototypes);
	void render(std::vector<TextureDisplay>& displays);
	
	void onMouseMove(SDL_MouseMotionEvent* event);
	
private:

	GPU_Target* m_screen;
	Renderer* m_renderer;
	Prototypes* m_prototypes;
	Point m_mouseLocation;
	
};