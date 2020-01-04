#include <CreepView.h>


void CreepView::render(Renderer* renderer, CreepState& creep, GameState* state, Prototypes* prototypes, int32_t thisPlayer)
{
	if (creep.creepProto_->moveType == MOVE_TYPE::TRACTOR)
	{
		float barrelAngle;
		float bodyAngle;
		bodyAngle = creep.orientation;
		
		if (creep.targetLoc_.getLength() > 0)
			barrelAngle = (creep.targetLoc_ - creep.unit.location).asAngle();
		else
			barrelAngle = bodyAngle;
					
		renderer->blit(*creep.creepProto_->hullTexture[creep.unit.force], creep.unit.location, bodyAngle, 1);
		renderer->blit(*creep.creepProto_->gunTexture[creep.unit.force], creep.unit.location, barrelAngle, 1);
		
		//GPU_Circle(renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, 1, ViewUtil::colorFromHex(0x00));
		/*
		auto scaledMovement = creep.unit.voluntaryMovement;
		if (scaledMovement.getLength() > 0)
		{
			scaledMovement.scaleTo(100);
			scaledMovement += creep.unit.location;
			
			GPU_Line(m_renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, scaledMovement.x, scaledMovement.y, color);			
		}
		if (creep.targetLoc_.getLength() > 0)
			GPU_Line(m_renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep.targetLoc_.x, creep.targetLoc_.y, color2);*/
	}
	else
	{
		float bodyAngle = creep.unit.voluntaryMovement.asAngle();
		float delta = FMath::angleDelta(bodyAngle, lastAngle);
		if (std::fabs(delta) > M_PI / (48.f + ((creep.unit.id * 7)% 24)) )
		{
			if (delta > 0)
				bodyAngle = lastAngle + M_PI / (48.f + ((creep.unit.id * 7) % 24));
			else
				bodyAngle = lastAngle - M_PI / (48.f + ((creep.unit.id * 7) % 24));
		}
		lastAngle = bodyAngle;
		renderer->blit(*creep.creepProto_->hullTexture[creep.unit.force], creep.unit.location, bodyAngle + M_PI_2, 0.5);
	}
}