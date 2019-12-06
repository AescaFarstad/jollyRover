#include <GenericRequestMessage.h>



GenericRequestMessage::GenericRequestMessage()
{
	typeId = MessageTypes::TYPE_REQUEST_MSG;
}


GenericRequestMessage::~GenericRequestMessage()
{
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

std::string GenericRequestMessage::getName()
{
	return "GenReq";
}
