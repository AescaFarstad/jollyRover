#include <GameStateMessage.h>

GameStateMessage::GameStateMessage()
{
	typeId = MESSAGE_TYPE::TYPE_GAME_STATE_MSG;
}

std::string GameStateMessage::getName() const
{
	return "GState";
}

void GameStateMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	serializer.write(*state, FIELD_NAME(state));
}

void GameStateMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	state = std::make_unique<GameState>();
	serializer.read(*state, FIELD_NAME(state));
}