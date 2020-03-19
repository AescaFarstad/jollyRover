#pragma once
#include <GameState.h>
#include <NetworkMessage.h>

class GameStateMessage :
	public NetworkMessage
{
public:
	GameStateMessage();

	std::unique_ptr<GameState> state;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};
