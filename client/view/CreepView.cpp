#include <CreepView.h>
#include <ViewUtil.h>

void CreepView::render(GPU_Target* screen, CreepState& creep, GameState* state, Prototypes* prototypes)
{
	SDL_Color color = colorFromHex(0xff0000);
	
	if (creep.unit.force == 0)
		GPU_Circle(screen, creep.unit.location.x, creep.unit.location.y, creep._creepProto->size, color);
	else
		GPU_CircleFilled(screen, creep.unit.location.x, creep.unit.location.y, creep._creepProto->size, color);
}