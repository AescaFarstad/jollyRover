
#include <DemoDataMessage.h>

DemoDataMessage::DemoDataMessage()
{
	typeId = MESSAGE_TYPE::TYPE_DEMO_DATA;
}

std::string DemoDataMessage::getName() const
{
	return "DemoDT";
}

void DemoDataMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, data);
}

void DemoDataMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, data);
}