#include <InputMessage.h>


void InputMessage::deserialize(SerializationStream& stream)
{
	NetworkMessage::deserialize(stream);
	Serializer::read(localId, stream);
	Serializer::read(serverId, stream);
	Serializer::read(serverStamp, stream);

}

void InputMessage::serialize(SerializationStream& stream) const
{
	NetworkMessage::serialize(stream);
	Serializer::write(localId, stream);
	Serializer::write(serverId, stream);
	Serializer::write(serverStamp, stream);
}

std::string InputMessage::getName() const
{
	return "I-abst";
}
