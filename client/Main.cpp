#include <Game.h>
#include <SystemInfo.h>
#include <FPSMeter.h>
#include <SDL_gpu.h>
#include <memory>

//#define WINDOWS 1
#define LINUX 1

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
	#include <emscripten/html5.h>
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
	const int32_t SCREEN_WIDTH = 1280;
	const int32_t SCREEN_HEIGHT = 720;
	const int32_t MAX_TIME_PER_FRAME = 100;
	const int32_t MIN_TIME_PER_FRAME = 5;

	GPU_Target* screen;
	
	Game game;

	bool isFinished = false;
	int32_t lastTicks = 0;
	
	std::string line = "";
	int32_t lineCount = 0;
}

using namespace MainInternal;

void mainLoop(void*)
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
			game.handleEvent(e);
		}
	}
	
	int32_t ticks = SDL_GetTicks();
	int32_t delta = ticks - lastTicks > MAX_TIME_PER_FRAME ? MAX_TIME_PER_FRAME : ticks - lastTicks;
	
	if (!isFinished && delta >= MIN_TIME_PER_FRAME)
	{
		//GPU_Clear(screen);

		lastTicks = ticks;
		game.update();
		S::fpsMeter.registerFrame(ticks);

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
		int32_t amaster;
		int32_t slave;
		termios params;
		openpty(&amaster, &slave, NULL, &params, NULL);
	#endif
#endif

	std::cout << "endiness: " << (SystemInfo::isBigEndian ? "big" : "little") << std::endl;
	
	GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
	screen = GPU_InitRenderer(GPU_RENDERER_GLES_2, SCREEN_WIDTH, SCREEN_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
	if (screen == NULL)
	{
		std::cout << "Failed to create window " << SDL_GetError() << std::endl;
		return 1;
	}
	
	printRenderers();
	printCurrentRenderer();
	
	SDL_Window* window = SDL_GetWindowFromID(screen->context->windowID);
	SDL_SetWindowPosition(window, S::config.window_X, S::config.window_Y);	
	
	game.init(screen);
	game.start();
	
#ifdef __EMSCRIPTEN__
	std::cout << "EMSCRIPTEN mode" << std::endl;
	emscripten_set_main_loop_arg(mainLoop, NULL, -1, true);	
#else
	std::cout << "Endless loop mode" << std::endl;
	while (!isFinished)
		mainLoop(NULL);
#endif

	return 0;
}

void printRenderers()
{
    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
    
    SDL_version compiled = GPU_GetCompiledVersion();
    SDL_version linked = GPU_GetLinkedVersion();
    if(compiled.major != linked.major || compiled.minor != linked.minor || compiled.patch != linked.patch)
        GPU_Log("SDL_gpu v%d.%d.%d (compiled with v%d.%d.%d)\n", linked.major, linked.minor, linked.patch, compiled.major, compiled.minor, compiled.patch);
    else
        GPU_Log("SDL_gpu v%d.%d.%d\n", linked.major, linked.minor, linked.patch);
    
	auto renderers = std::make_unique<GPU_RendererID[]>(GPU_GetNumRegisteredRenderers());
	GPU_GetRegisteredRendererList(renderers.get());
	GPU_Log("\nAvailable renderers:\n");
	for(int32_t i = 0; i < GPU_GetNumRegisteredRenderers(); i++)
	{
		GPU_Log("* %s (%d.%d)\n", renderers[i].name, renderers[i].major_version, renderers[i].minor_version);
	}

	GPU_RendererID order[GPU_RENDERER_ORDER_MAX];
	int32_t order_size;
	GPU_GetRendererOrder(&order_size, order);
	GPU_Log("Renderer init order:\n");
	for(int32_t i = 0; i < order_size; i++)
	{
		GPU_Log("%d) %s (%d.%d)\n", i+1, order[i].name, order[i].major_version, order[i].minor_version);
	}

	GPU_Log("\n");
}

void printCurrentRenderer()
{
    GPU_Renderer* renderer = GPU_GetCurrentRenderer();
    GPU_RendererID id = renderer->id;
    
	GPU_Log("Using renderer: %s (%d.%d)\n", id.name, id.major_version, id.minor_version);
	//GPU_Log(" Shader versions supported: %d to %d\n\n", renderer->min_shader_version, renderer->max_shader_version);
}