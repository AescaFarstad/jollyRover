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
	}
	else
	{
		float bodyAngle = creep.unit.voluntaryMovement.asAngle();
		float delta = FMath::angleDelta(bodyAngle, m_lastAngle);
		if (std::fabs(delta) > M_PI / (48.f + ((creep.unit.id * 7)% 24)) )
		{
			if (delta > 0)
				bodyAngle = m_lastAngle + M_PI / (48.f + ((creep.unit.id * 7) % 24));
			else
				bodyAngle = m_lastAngle - M_PI / (48.f + ((creep.unit.id * 7) % 24));
		}
		m_lastAngle = bodyAngle;
		renderer->blit(*creep.creepProto_->hullTexture[creep.unit.force], creep.unit.location, bodyAngle + M_PI_2, 0.5);
	}
}

float CreepView::getRotation()
{
	return m_lastAngle + M_PI_2;
}