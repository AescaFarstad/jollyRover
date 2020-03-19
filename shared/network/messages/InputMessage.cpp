#include <InputMessage.h>


std::string InputMessage::getName() const
{
	return "I-abst";
}

void InputMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, serverStamp);
	WRITE_FIELD((*this), serializer, localId);
	WRITE_FIELD((*this), serializer, serverId);
}

void InputMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, serverStamp);
	READ__FIELD((*this), serializer, localId);
	READ__FIELD((*this), serializer, serverId);
}
