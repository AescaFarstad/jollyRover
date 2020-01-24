#include <GameStateMessage.h>

GameStateMessage::GameStateMessage()
{
	typeId = MESSAGE_TYPE::TYPE_GAME_STATE_MSG;
}

void GameStateMessage::deserialize(SerializationStream& stream)
{
	NetworkMessage::deserialize(stream);
	state = std::make_unique<GameState>();
	state->deserialize(stream);
}

void GameStateMessage::serialize(SerializationStream& stream) const
{
	NetworkMessage::serialize(stream);
	state->serialize(stream);
}

std::string GameStateMessage::getName() const
{
	return "GState";
}
