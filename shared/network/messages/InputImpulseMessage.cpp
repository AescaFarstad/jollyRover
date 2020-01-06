#include <InputImpulseMessage.h>


InputImpulseMessage::InputImpulseMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG;
	impulse = INPUT_IMPULSE::NONE;
}

void InputImpulseMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(impulse, stream);
}

void InputImpulseMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(impulse, stream);
}

std::string InputImpulseMessage::getName() const
{
	return "I-impl";
}