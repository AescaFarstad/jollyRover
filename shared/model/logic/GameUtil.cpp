#include <GameUtil.h>

namespace GameUtil 
{
	Unit* resolveTarget(Target& target, GameState* state)
	{
		if (target.type == ENTTITY_TYPE::CREEP)
		{
			auto creep = state->creepById_[target.id];
			return creep ? &creep->unit : nullptr;
		}
		if (target.type == ENTTITY_TYPE::CAR)
		{
			//TODO optimise
			for(auto& player : state->players)
			{
				for(auto& car : player.activeCars)
				{
					if (car.unit.id == target.id)
						return &car.unit;
				}
			}
			return nullptr;
		}
		THROW_FATAL_ERROR("Unknown target type");
		return nullptr;
	}
	
	std::vector<FormationState>::iterator formationById(int32_t id, GameState* state)
	{
		return std::find_if(state->formations.begin(), state->formations.end(), [id](FormationState& f) {
					return f.id == id;
					});
	}
}