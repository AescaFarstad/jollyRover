
#include <Game.h>
#include <SystemInfo.h>
#include <FPSMeter.h>

//#define WINDOWS 1
#define LINUX 1

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <SDL.h>
#elif LINUX
	#include <pty.h>
	#include <SDL2/SDL.h>
#else
	#include <windows.h>
	#include <SDL2/SDL.h>
#endif

namespace MainInternal
{

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int MAX_TIME_PER_FRAME = 100;
const int MIN_TIME_PER_FRAME = 10;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Game* game;

bool isFinished = false;
int lastTicks = 0;
FPSMeter fpsMeter;
}
using namespace MainInternal;

void mainLoop(void* arg)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			SDL_DestroyWindow(window);
			SDL_Quit();
			isFinished = true;
			break;
		}
		else
		{
			game->handleEvent(&e);
		}
	}
	int ticks = SDL_GetTicks();
	int delta = ticks - lastTicks > MAX_TIME_PER_FRAME ? MAX_TIME_PER_FRAME : ticks - lastTicks;
	if (delta >= MIN_TIME_PER_FRAME)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		lastTicks = ticks;
		game->update();/*
		fpsMeter.registerFrame(ticks);
		if (fpsMeter.getMeasurementDuration() > 5000)
			printf(fpsMeter.report().c_str());*/

		SDL_RenderPresent(renderer);
	}
}

int main(int argc, char* args[])
{
#ifndef __EMSCRIPTEN__
	#ifdef WINDOWS
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	#elif LINUX
		int amaster;
		int slave;
		termios params;
		openpty(&amaster, &slave, NULL, &params, NULL);
	#endif
#endif
	printf("endiness: %s\n", (SystemInfo::instance->isBigEndian ? "big" : "little"));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to initialize SDL %s\n", SDL_GetError());
		return 0;
	}
	
	window = SDL_CreateWindow("smth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_SetWindowPosition(window, 640, 50);
	if (window == NULL)
	{
		printf("Failed to create window %s\n", SDL_GetError());
		return 0;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Failed to create renderer %s\n", SDL_GetError());
		return 0;
	}

	S::log.disableTags({
		//LOG_TAGS::UNIQUE,
		LOG_TAGS::NET,
		LOG_TAGS::INPUT_,
		//LOG_TAGS::GAME,

		LOG_TAGS::NET_MESSAGE,
		LOG_TAGS::Z
	});

	S::log.enableTags({
		//LOG_TAGS::UNIQUE,
		//LOG_TAGS::NET,
		//LOG_TAGS::GAME,
		LOG_TAGS::ERROR_,

		//LOG_TAGS::NET_MESSAGE,
		LOG_TAGS::Z
	});
	
	game = new Game(window, renderer);
	
#ifdef __EMSCRIPTEN__
	printf("EMSCRIPTEN mode\n");
	emscripten_set_main_loop_arg(mainLoop, NULL, -1, true);
#else
	printf("Endless loop mode\n");
	while (!isFinished)
		mainLoop(NULL);
#endif

	return 0;
}