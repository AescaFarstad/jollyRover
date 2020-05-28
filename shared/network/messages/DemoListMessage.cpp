
#include <DemoListMessage.h>

DemoListMessage::DemoListMessage()
{
	typeId = MESSAGE_TYPE::TYPE_DEMO_LIST;
}

std::string DemoListMessage::getName() const
{
	return "DemoLS";
}

void DemoListMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, list);
}

void DemoListMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, list);
}