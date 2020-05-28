
#include <DemoRequestMessage.h>

DemoRequestMessage::DemoRequestMessage()
{
	typeId = MESSAGE_TYPE::TYPE_DEMO_REQUEST;
}

std::string DemoRequestMessage::getName() const
{
	return "DemoRQ";
}

void DemoRequestMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, demoName);
}

void DemoRequestMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, demoName);
}