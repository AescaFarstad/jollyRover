#include <InputPlayerJoinedMessage.h>



InputPlayerJoinedMessage::InputPlayerJoinedMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG;
}

std::string InputPlayerJoinedMessage::getName() const
{
	return "I-join";
}

void InputPlayerJoinedMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, login);
}

void InputPlayerJoinedMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, login);
}
