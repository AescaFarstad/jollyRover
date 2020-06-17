#include <GenericRequestMessage.h>

GenericRequestMessage::GenericRequestMessage()
{
	typeId = MESSAGE_TYPE::TYPE_REQUEST_MSG;
	request = REQUEST_TYPE::INVALID;
}

std::string GenericRequestMessage::getName() const
{
	return "GenReq";
}

void GenericRequestMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, request);
}

void GenericRequestMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, request);
}
