#include <InputPlayerLeftMessage.h>



InputPlayerLeftMessage::InputPlayerLeftMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG;
}

std::string InputPlayerLeftMessage::getName() const
{
	return "I-left";
}

void InputPlayerLeftMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, login);
}

void InputPlayerLeftMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, login);
}
