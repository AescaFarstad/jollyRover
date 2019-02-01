#include <InputPlayerLeftMessage.h>



InputPlayerLeftMessage::InputPlayerLeftMessage()
{
	typeId = MessageTypes::TYPE_INPUT_LEFT_MSG;
}


InputPlayerLeftMessage::~InputPlayerLeftMessage()
{
}

void InputPlayerLeftMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(login, stream);
}

void InputPlayerLeftMessage::serialize(SerializationStream& stream)
{
	InputMessage::serialize(stream);
	Serializer::write(login, stream);
}

std::string InputPlayerLeftMessage::getName()
{
	return "I-left";
}
