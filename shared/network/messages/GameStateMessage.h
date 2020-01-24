#pragma once
#include <NetworkMessage.h>
#include <GameState.h>

class GameStateMessage :
	public NetworkMessage
{
public:
	GameStateMessage();

	std::unique_ptr<GameState> state;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};

