#include <InputDebugMessage.h>

InputDebugMessage::InputDebugMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG;
	action = DEBUG_ACTION::NONE;
}

std::string InputDebugMessage::getName() const
{
	return "I-dbga";
}

void InputDebugMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, action);
	WRITE_FIELD((*this), serializer, coords);
}

void InputDebugMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, action);
	READ__FIELD((*this), serializer, coords);
}
