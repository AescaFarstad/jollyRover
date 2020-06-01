#include <Root.h>

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

	Root root;
	root.init();
	
	#ifdef __EMSCRIPTEN__
		std::cout << "EMSCRIPTEN mode" << std::endl;
		emscripten_set_main_loop_arg(Root::mainLoop, NULL, -1, true);	
	#else
		std::cout << "Endless loop mode" << std::endl;
		while (!root.mainLoop())
			;
	#endif

	return 0;
}

