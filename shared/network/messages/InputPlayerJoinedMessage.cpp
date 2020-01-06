#include <InputPlayerJoinedMessage.h>



InputPlayerJoinedMessage::InputPlayerJoinedMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG;
}

void InputPlayerJoinedMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(login, stream);
}

void InputPlayerJoinedMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(login, stream);
}

std::string InputPlayerJoinedMessage::getName() const
{
	return "I-join";
}
