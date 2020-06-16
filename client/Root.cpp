#include <Root.h>
#include <SystemInfo.h>
#include <FPSMeter.h>

static Root* root;

void Root::init()
{
	std::cout << "endiness: " << (SystemInfo::isBigEndian ? "big" : "little") << std::endl;
	
	GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
	m_screen = GPU_InitRenderer(GPU_RENDERER_GLES_2, SCREEN_WIDTH, SCREEN_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
	if (m_screen == NULL)
	{
		std::cout << "Failed to create window " << SDL_GetError() << std::endl;
		return;
	}
	
	//printRenderers();
	printCurrentRenderer();
	
	//Temporary setup until config file is parsed
	Point field{1280, 720};
	SDL_Window* window = SDL_GetWindowFromID(m_screen->context->windowID);
	SDL_SetWindowSize(window, field.x, field.y);
	GPU_SetWindowResolution(field.x, field.y);
	SDL_SetWindowPosition(window, -20, 250);
	GPU_RectangleFilled(m_screen, 0, 0, field.x, field.y, ViewUtil::colorFromHex(0xffff77, 0xff));
	GPU_Flip(m_screen);
	
	m_game.init(m_screen);
	m_game.start();
	
	root = this;
	
	S::isLoaded = true;
}

void Root::mainLoop(void*)
{
	root->mainLoop();
}

bool Root::mainLoop()
{
	bool isFinished = false;
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
			m_game.handleEvent(e);
		}
	}
	
	int32_t ticks = SDL_GetTicks();
	int32_t delta = ticks - m_lastTicks > MAX_TIME_PER_FRAME ? MAX_TIME_PER_FRAME : ticks - m_lastTicks;
	
	if (!isFinished && delta >= MIN_TIME_PER_FRAME)
	{
		//GPU_Clear(screen);

		m_lastTicks = ticks;
		m_game.update();
		S::fpsMeter.registerFrame(ticks);

		GPU_Flip(m_screen);
	}
	return isFinished;
}

void Root::printRenderers()
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

void Root::printCurrentRenderer()
{
    GPU_Renderer* renderer = GPU_GetCurrentRenderer();
    GPU_RendererID id = renderer->id;
    
	GPU_Log("Using renderer: %s (%d.%d)\n", id.name, id.major_version, id.minor_version);
	//GPU_Log(" Shader versions supported: %d to %d\n\n", renderer->min_shader_version, renderer->max_shader_version);
}