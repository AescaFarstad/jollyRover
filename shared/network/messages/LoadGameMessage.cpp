#include <LoadGameMessage.h>

LoadGameMessage::LoadGameMessage()
{
	typeId = MESSAGE_TYPE::TYPE_LOAD_GAME_MSG;
}

std::string LoadGameMessage::getName() const
{
	return "I-Load";
}

void LoadGameMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, state);
}

void LoadGameMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, state);
}