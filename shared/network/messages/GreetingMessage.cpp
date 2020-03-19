#include <GreetingMessage.h>

GreetingMessage::GreetingMessage()
{
	typeId = MESSAGE_TYPE::TYPE_GREETING_MSG;
	login = 0;
}

std::string GreetingMessage::getName() const
{
	return "greetM";
}

void GreetingMessage::serialize(BinarySerializer& serializer) const
{
	NetworkMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, login);
	WRITE_FIELD((*this), serializer, password);
}

void GreetingMessage::deserialize(BinarySerializer& serializer)
{
	NetworkMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, login);
	READ__FIELD((*this), serializer, password);
}
