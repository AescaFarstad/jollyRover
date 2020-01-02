#pragma once
#include <GameState.h>

namespace GameUtil
{
	Unit* resolveTarget(Target& target, GameState* state);
	
	std::vector<FormationState>::iterator formationById(int32_t id, GameState* state);
}