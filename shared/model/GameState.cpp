#include <GameState.h>

GameState::GameState()
{
	timeStamp = 0;
}

GameState::~GameState()
{
}

void GameState::deserialize(SerializationStream & stream)
{
	Serializer::read(timeStamp, stream);
	Serializer::read(random, stream);
	Serializer::read(stepsPerformed, stream);
	Serializer::readVector(players, stream);
}

void GameState::serialize(SerializationStream & stream)
{
	Serializer::write(timeStamp, stream);
	Serializer::write(random, stream);
	Serializer::write(stepsPerformed, stream);
	Serializer::writeVector(players, stream);
}
