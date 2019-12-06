#include <GreetingMessage.h>



GreetingMessage::GreetingMessage()
{
	typeId = MessageTypes::TYPE_GREETING_MSG;
	login = 0;
}


GreetingMessage::~GreetingMessage()
{
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

std::string GreetingMessage::getName()
{
	return "greetM";
}
