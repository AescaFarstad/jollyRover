#include <CreepView.h>
#include <ViewUtil.h>

void CreepView::render(Renderer* renderer, CreepState& creep, GameState* state, Prototypes* prototypes)
{
	SDL_Color color = colorFromHex(0xff0000);
	//SDL_Color color2 = colorFromHex(0x0000ff);
	if (creep.object.prototypeId != 0)
	{
		if (creep.unit.force == 0)
			GPU_Circle(renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep._creepProto->size, color);
		else
			GPU_CircleFilled(renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep._creepProto->size, color);
	}
	else
	{
		float barrelAngle;
		float bodyAngle = creep.unit.voluntaryMovement.asAngleInDegrees();
		
		if (creep.targetLoc_.getLength() > 0)
			barrelAngle = (creep.targetLoc_ - creep.unit.location).asAngleInDegrees();
		else
			barrelAngle = bodyAngle;
		
		renderer->blit(S::textures.tanks_2.tankBody_sand, creep.unit.location, bodyAngle, 1);
		renderer->blit(S::textures.tanks_2.tankSand_barrel1, creep.unit.location, barrelAngle, 1);
		/*
		auto scaledMovement = creep.unit.voluntaryMovement;
		if (scaledMovement.getLength() > 0)
		{
			scaledMovement.scaleTo(100);
			scaledMovement += creep.unit.location;
			
			GPU_Line(renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, scaledMovement.x, scaledMovement.y, color);			
		}
		if (creep.targetLoc_.getLength() > 0)
			GPU_Line(renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep.targetLoc_.x, creep.targetLoc_.y, color2);*/
	}
}