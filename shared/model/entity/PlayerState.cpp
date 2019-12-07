#include <PlayerState.h>


void PlayerState::deserialize(SerializationStream & stream)
{
	Serializer::read(login, stream);
	Serializer::read(score, stream);
	Serializer::read(isHeadless, stream);
	Serializer::read(isAI, stream);
	Serializer::readVector(activeCars, stream);
}

void PlayerState::serialize(SerializationStream & stream) const
{
				
	Serializer::write(login, stream);
	Serializer::write(score, stream);
	Serializer::write(isHeadless, stream);
	Serializer::write(isAI, stream);
	Serializer::writeVector(activeCars, stream);
}
