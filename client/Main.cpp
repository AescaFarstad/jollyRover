
#include <Game.h>
#include <SystemInfo.h>
#include <FPSMeter.h>
#include <SDL_gpu.h>

//#define WINDOWS 1
#define LINUX 1

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <SDL.h>
#elif LINUX
	#include <pty.h>
	#include <SDL.h>
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

	GPU_Target* screen;
	
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
			isFinished = true;
			GPU_Quit();
			break;
		}
		else
		{
			game->handleEvent(&e);
		}
	}
	int ticks = SDL_GetTicks();
	int delta = ticks - lastTicks > MAX_TIME_PER_FRAME ? MAX_TIME_PER_FRAME : ticks - lastTicks;
	if (!isFinished && delta >= MIN_TIME_PER_FRAME)
	{
		GPU_Clear(screen);

		lastTicks = ticks;
		game->update();/*
		fpsMeter.registerFrame(ticks);
		if (fpsMeter.getMeasurementDuration() > 5000)
			printf(fpsMeter.report().c_str());*/

		GPU_Flip(screen);
	}
}

void printRenderers();
void printCurrentRenderer();

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
	screen = GPU_InitRenderer(GPU_RENDERER_GLES_2, SCREEN_WIDTH, SCREEN_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
	printRenderers();
	printCurrentRenderer();
	SDL_Window* window = SDL_GetWindowFromID(screen->context->windowID);
	SDL_SetWindowPosition(window, S::config.window_X, S::config.window_Y);
	
	if (screen == NULL)
	{
		printf("Failed to create window %s\n", SDL_GetError());
		return 0;
	}
	
	game = new Game(screen);
	
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

void printRenderers()
{
	SDL_version compiled;
	SDL_version linked;
	GPU_RendererID* renderers;
	int i;
	int order_size;
	GPU_RendererID order[GPU_RENDERER_ORDER_MAX];

    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
    
    compiled = GPU_GetCompiledVersion();
    linked = GPU_GetLinkedVersion();
    if(compiled.major != linked.major || compiled.minor != linked.minor || compiled.patch != linked.patch)
        GPU_Log("SDL_gpu v%d.%d.%d (compiled with v%d.%d.%d)\n", linked.major, linked.minor, linked.patch, compiled.major, compiled.minor, compiled.patch);
    else
        GPU_Log("SDL_gpu v%d.%d.%d\n", linked.major, linked.minor, linked.patch);
    
	renderers = (GPU_RendererID*)malloc(sizeof(GPU_RendererID)*GPU_GetNumRegisteredRenderers());
	GPU_GetRegisteredRendererList(renderers);
	
	GPU_Log("\nAvailable renderers:\n");
	for(i = 0; i < GPU_GetNumRegisteredRenderers(); i++)
	{
		GPU_Log("* %s (%d.%d)\n", renderers[i].name, renderers[i].major_version, renderers[i].minor_version);
	}
	GPU_Log("Renderer init order:\n");
	
	GPU_GetRendererOrder(&order_size, order);
	for(i = 0; i < order_size; i++)
	{
		GPU_Log("%d) %s (%d.%d)\n", i+1, order[i].name, order[i].major_version, order[i].minor_version);
	}
	GPU_Log("\n");

	free(renderers);
}

void printCurrentRenderer()
{
    GPU_Renderer* renderer = GPU_GetCurrentRenderer();
    GPU_RendererID id = renderer->id;
    
	GPU_Log("Using renderer: %s (%d.%d)\n", id.name, id.major_version, id.minor_version);
	//GPU_Log(" Shader versions supported: %d to %d\n\n", renderer->min_shader_version, renderer->max_shader_version);
}