#pragma once
#include <PlayerTest.h>
#include <SeededRandom.h>
#include <ISerializable.h>
#include <Creep.h>
#include <Projectile.h>
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
	
	std::vector<Creep> creeps;
	std::vector<Projectile> projectiles;
	//std::vector<ForceState> forces;
	
	std::vector<PlayerTest> players;
	SeededRandom random;
	uint32_t timeStamp;
	int16_t stepsPerformed;
	uint32_t idCounter;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};