#include <InputActionMessage.h>

InputActionMessage::InputActionMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG;
}

std::string InputActionMessage::getName() const
{
	return "I-actn";
}

void InputActionMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, angle);
	WRITE_FIELD((*this), serializer, downedButtons);
	WRITE_FIELD((*this), serializer, uppedButtons);
}

void InputActionMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, angle);
	READ__FIELD((*this), serializer, downedButtons);
	READ__FIELD((*this), serializer, uppedButtons);
}