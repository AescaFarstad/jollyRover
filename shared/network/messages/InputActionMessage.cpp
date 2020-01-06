#include <InputActionMessage.h>



InputActionMessage::InputActionMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG;
}


InputActionMessage::~InputActionMessage()
{
}

void InputActionMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(angle, stream);
	Serializer::readVector(downedButtons, stream);
	Serializer::readVector(uppedButtons, stream);
}

void InputActionMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(angle, stream);
	Serializer::writeVector(downedButtons, stream);
	Serializer::writeVector(uppedButtons, stream);
}

std::string InputActionMessage::getName() const
{
	return "I-actn";
}
