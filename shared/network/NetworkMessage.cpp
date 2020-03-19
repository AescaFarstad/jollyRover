#include <NetworkMessage.h>
#include <sstream>


int32_t NetworkMessage::idCounter = 1;

NetworkMessage::NetworkMessage()
{
	initiator_id = getMessageId();
	typeId = MESSAGE_TYPE::TYPE_UNINITIALIZED;
	stamp = 0;
	inResponseTo = -1;
}

std::string NetworkMessage::getName() const
{
	return "NetMsg";
}

int32_t NetworkMessage::getMessageId()
{
	return idCounter++;
}

void NetworkMessage::serialize(BinarySerializer& serializer) const
{
	WRITE_FIELD((*this), serializer, typeId);
	WRITE_FIELD((*this), serializer, initiator_id);
	WRITE_FIELD((*this), serializer, login);
	WRITE_FIELD((*this), serializer, stamp);
	WRITE_FIELD((*this), serializer, inResponseTo);
}

void NetworkMessage::deserialize(BinarySerializer& serializer)
{
	READ__FIELD((*this), serializer, typeId);
	READ__FIELD((*this), serializer, initiator_id);
	READ__FIELD((*this), serializer, login);
	READ__FIELD((*this), serializer, stamp);
	READ__FIELD((*this), serializer, inResponseTo);
}
