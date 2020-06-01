#pragma once
#include <SDL_gpu.h>
#include <Game.h>
#include <string>

class Root
{
public:
	Root() = default;
	
	static void mainLoop(void*);
	
	void init();
	bool mainLoop();
	
private:
	constexpr static int32_t SCREEN_WIDTH = 1280;
	constexpr static int32_t SCREEN_HEIGHT = 720;
	constexpr static int32_t MAX_TIME_PER_FRAME = 100;
	constexpr static int32_t MIN_TIME_PER_FRAME = 5;

	GPU_Target* m_screen;	
	Game m_game;
	int32_t m_lastTicks = 0;	
	
	void printRenderers();
	void printCurrentRenderer();

};
