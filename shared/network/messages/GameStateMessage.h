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

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() override;
};

