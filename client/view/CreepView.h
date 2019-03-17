#include <GameState.h>
#include <Prototypes.h>
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
	
	int16_t id;
	
	void render(SDL_Renderer* renderer, CreepState& creep, GameState* state, Prototypes* prototypes);
	
	
private:

};
