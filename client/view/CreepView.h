#include <GameState.h>
#include <Prototypes.h>
#include <SDL_gpu.h>
#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif

class CreepView
{
public:
	CreepView() = default;
	~CreepView() = default;
	
	int16_t m_id;
	
	void render(GPU_Target* screen, CreepState& creep, GameState* state, Prototypes* prototypes);
	
	
private:

};
