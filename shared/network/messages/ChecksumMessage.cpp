#include <ChecksumMessage.h>

ChecksumMessage::ChecksumMessage()
{
	typeId = MESSAGE_TYPE::TYPE_CHECKSUM_MSG;
}

std::string ChecksumMessage::getName() const
{
	return "CRCmsg";
}

void ChecksumMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, checksums);
}

void ChecksumMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, checksums);
}