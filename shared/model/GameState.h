#pragma once
#include <PlayerTest.h>
#include <SeededRandom.h>
#include <ISerializable.h>
#include <CreepState.h>
#include <Projectile.h>
#include <FormationState.h>
#include <vector>
/*
class ForceState
{
	ForceState() = default;
	~ForceState() = default;
	
	
	
};*/

class GameState
{
public:
	GameState();
	~GameState() = default;
	
	std::vector<CreepState> creeps;
	std::vector<Projectile> projectiles;
	//std::vector<ForceState> forces;
	
	std::vector<PlayerTest> players;
	std::vector<FormationState> formations;
	SeededRandom random;
	uint32_t timeStamp;
	int16_t stepsPerformed;
	uint32_t idCounter;
	
	std::vector<int> _forceStrength;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};