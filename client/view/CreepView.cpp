#include <CreepView.h>


void CreepView::render(SDL_Renderer* renderer, CreepState& creep, GameState* state, Prototypes* prototypes)
{
	uint32_t color = 0xff0000;
	int rectSize1 = 10;
	int rectSize2 = 6;
	SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0xFF);
	
	int rectSize = creep.weapon.prototypeId == 1 ? rectSize1 : rectSize2;
	SDL_Rect rect;
	rect.x = creep.unit.location.x - rectSize/2;
	rect.y = creep.unit.location.y - rectSize/2;
	rect.w = rectSize;
	rect.h = rectSize;
	SDL_RenderDrawRect(renderer, &rect);
	
	if (creep.unit.force == 1)
	{
		rectSize -= 2;		
		
		rect.x = creep.unit.location.x - rectSize/2;
		rect.y = creep.unit.location.y - rectSize/2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(renderer, &rect);
		
		rectSize -= 2;		
		
		rect.x = creep.unit.location.x - rectSize/2;
		rect.y = creep.unit.location.y - rectSize/2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(renderer, &rect);
		
	}
}