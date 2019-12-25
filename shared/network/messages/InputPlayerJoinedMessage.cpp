#include <InputPlayerJoinedMessage.h>



InputPlayerJoinedMessage::InputPlayerJoinedMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG;
}


InputPlayerJoinedMessage::~InputPlayerJoinedMessage()
{
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

std::string InputPlayerJoinedMessage::getName()
{
	return "I-join";
}
