#include <NetworkMessage.h>
#include <sstream>


int32_t NetworkMessage::idCounter = 1;

NetworkMessage::NetworkMessage()
{
	initiator_id = -1;
	typeId = MESSAGE_TYPE::TYPE_UNINITIALIZED;
	stamp = 0;
	inResponseTo = -1;
}


NetworkMessage::~NetworkMessage()
{
}

void NetworkMessage::deserialize(SerializationStream& stream)
{
	Serializer::read(typeId, stream);
	Serializer::read(initiator_id, stream);
	Serializer::read(login, stream);
	Serializer::read(stamp, stream);
	Serializer::read(inResponseTo, stream);
}

void NetworkMessage::serialize(SerializationStream& stream) const
{
	Serializer::write(typeId, stream);
	Serializer::write(initiator_id, stream);
	Serializer::write(login, stream);
	Serializer::write(stamp, stream);
	Serializer::write(inResponseTo, stream);
}

std::string NetworkMessage::getName()
{
	return "NetMsg";
}

int32_t NetworkMessage::getMessageId()
{
	return idCounter++;
}
