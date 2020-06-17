#pragma once
#include <Renderer.h>
#include <GameState.h>

class CreepView
{
public:
	uint32_t lastUpdate;
	
	void render(Renderer* renderer, CreepState& creep, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	float getRotation();
	
private:
	float m_lastAngle = 0;
};