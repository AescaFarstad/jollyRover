#include <GameState.h>

GameState::GameState()
{
	timeStamp = 0;
	idCounter = 1;
	loadCount = 0;
	isEventLoggerEnabled = false;
	creeps.reserve(MAX_CREEPS);
}

GameState::GameState(uint32_t randomSeed) : GameState()
{
	random.initFromSeed(randomSeed);
}

void GameState::propagatePrototypes(Prototypes* prototypes)
{
	for(auto& creep : creeps)
	{
		creep.propagatePrototypes(prototypes->creeps, prototypes->weapons);
	}
	for(auto& formation : formations)
	{
		formation.propagatePrototypes(prototypes->formations);
	}
}
