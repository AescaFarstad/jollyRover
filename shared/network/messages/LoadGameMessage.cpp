#include <LoadGameMessage.h>

LoadGameMessage::LoadGameMessage()
{
	typeId = MESSAGE_TYPE::TYPE_LOAD_GAME_MSG;
	state = nullptr;
}

void LoadGameMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(state, stream);
}

void LoadGameMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(state, stream);
}

std::string LoadGameMessage::getName()
{
	return "I-Load";
}