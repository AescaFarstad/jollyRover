#include <PlayerState.h>


void PlayerTest::deserialize(SerializationStream & stream)
{
	Serializer::read(login, stream);
	Serializer::read(isHeadless, stream);
	Serializer::read(isAI, stream);
	Serializer::readVector(activeCars, stream);
}

void PlayerTest::serialize(SerializationStream & stream) const
{
				
	Serializer::write(login, stream);
	Serializer::write(isHeadless, stream);
	Serializer::write(isAI, stream);
	Serializer::writeVector(activeCars, stream);
}
