#pragma once
#include <PlayerTest.h>
#include <SeededRandom.h>
#include <ISerializable.h>
#include <vector>

class GameState : 
	public ISerializable
{
public:
	GameState();
	~GameState();
	std::vector<PlayerTest> players;
	SeededRandom random;
	uint32_t timeStamp;
	int16_t stepsPerformed;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);

private:

};