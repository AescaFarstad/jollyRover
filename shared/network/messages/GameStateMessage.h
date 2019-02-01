#pragma once
#include <NetworkMessage.h>
#include <GameState.h>

class GameStateMessage :
	public NetworkMessage
{
public:
	GameStateMessage();
	~GameStateMessage();

	GameState* state;
	bool ownsState;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

