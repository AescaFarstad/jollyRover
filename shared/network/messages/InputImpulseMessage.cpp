#include <InputImpulseMessage.h>


InputImpulseMessage::InputImpulseMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG;
	impulse = INPUT_IMPULSE::NONE;
}

std::string InputImpulseMessage::getName() const
{
	return "I-impl";
}

void InputImpulseMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, impulse);
}

void InputImpulseMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, impulse);
}