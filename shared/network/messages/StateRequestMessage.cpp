#include <StateRequestMessage.h>

StateRequestMessage::StateRequestMessage()
{
	typeId = MESSAGE_TYPE::TYPE_STATE_REQUEST_MSG;
}

std::string StateRequestMessage::getName() const
{
	return "RState";
}

void StateRequestMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, states);
}

void StateRequestMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, states);
}