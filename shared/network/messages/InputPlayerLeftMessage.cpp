#include <InputPlayerLeftMessage.h>



InputPlayerLeftMessage::InputPlayerLeftMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG;
}


InputPlayerLeftMessage::~InputPlayerLeftMessage()
{
}

void InputPlayerLeftMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(login, stream);
}

void InputPlayerLeftMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(login, stream);
}

std::string InputPlayerLeftMessage::getName() const
{
	return "I-left";
}
