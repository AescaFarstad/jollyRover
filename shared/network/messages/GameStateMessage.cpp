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
	WRITE_FIELD((*this), serializer, states);
}

void GameStateMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, states);
}