#include <GenericRequestMessage.h>



GenericRequestMessage::GenericRequestMessage()
{
	typeId = MESSAGE_TYPE::TYPE_REQUEST_MSG;
}

void GenericRequestMessage::deserialize(SerializationStream& stream)
{
	NetworkMessage::deserialize(stream);
	Serializer::read(request, stream);
}

void GenericRequestMessage::serialize(SerializationStream& stream) const
{
	NetworkMessage::serialize(stream);
	Serializer::write(request, stream);
}

std::string GenericRequestMessage::getName() const
{
	return "GenReq";
}
