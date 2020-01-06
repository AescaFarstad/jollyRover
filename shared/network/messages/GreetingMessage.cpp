#include <GreetingMessage.h>



GreetingMessage::GreetingMessage()
{
	typeId = MESSAGE_TYPE::TYPE_GREETING_MSG;
	login = 0;
}

void GreetingMessage::deserialize(SerializationStream& stream)
{
	NetworkMessage::deserialize(stream);
	Serializer::read(login, stream);
	Serializer::readVector(password, stream);
}

void GreetingMessage::serialize(SerializationStream& stream) const
{
	NetworkMessage::serialize(stream);
	Serializer::write(login, stream);
	Serializer::writeVector(password, stream);
}

std::string GreetingMessage::getName() const
{
	return "greetM";
}
