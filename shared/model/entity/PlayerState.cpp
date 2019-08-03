#include <PlayerState.h>


void PlayerTest::deserialize(SerializationStream & stream)
{
	Serializer::read(login, stream);
	Serializer::readVector(activeCars, stream);
}

void PlayerTest::serialize(SerializationStream & stream) const
{
				
	Serializer::write(login, stream);
	Serializer::writeVector(activeCars, stream);
}
