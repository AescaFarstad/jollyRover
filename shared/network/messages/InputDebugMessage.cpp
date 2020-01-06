#include <InputDebugMessage.h>

InputDebugMessage::InputDebugMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG;
	action = DEBUG_ACTION::NONE;
}

void InputDebugMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(action, stream);
	Serializer::read(coords, stream);
}

void InputDebugMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(action, stream);
	Serializer::write(coords, stream);
}

std::string InputDebugMessage::getName() const
{
	return "I-dbga";
}