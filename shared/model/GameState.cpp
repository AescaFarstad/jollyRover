#include <GameState.h>

GameState::GameState()
{
	timeStamp = 0;
	idCounter = 1;
	stepsPerformed = 0;
}

GameState::GameState(uint32_t randomSeed) : GameState()
{
	random.initFromSeed(randomSeed);
}

void GameState::deserialize(SerializationStream & stream)
{
	Serializer::read(timeStamp, stream);
	Serializer::read(random, stream);
	Serializer::read(stepsPerformed, stream);
	Serializer::read(idCounter, stream);
	Serializer::readVector(players, stream);
	Serializer::readVector(creeps, stream);
	Serializer::readVector(projectiles, stream);
	Serializer::readVector(formations, stream);
}

void GameState::serialize(SerializationStream & stream) const 
{
	Serializer::write(timeStamp, stream);
	Serializer::write(random, stream);
	Serializer::write(stepsPerformed, stream);
	Serializer::write(idCounter, stream);
	Serializer::writeVector(players, stream);
	Serializer::writeVector(creeps, stream);
	Serializer::writeVector(projectiles, stream);
	Serializer::writeVector(formations, stream);
}
