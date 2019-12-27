#include <PlayerState.h>


void PlayerState::deserialize(SerializationStream & stream)
{
	Serializer::read(login, stream);
	Serializer::read(score, stream);
	Serializer::read(isHeadless, stream);
	Serializer::read(isAI, stream);
	Serializer::read(repairsTotal, stream);
	Serializer::read(repairsLeft, stream);
	Serializer::read(refuelTotal, stream);
	Serializer::read(refuelLeft, stream);
	Serializer::readVector(activeCars, stream);
}

void PlayerState::serialize(SerializationStream & stream) const
{
				
	Serializer::write(login, stream);
	Serializer::write(score, stream);
	Serializer::write(isHeadless, stream);
	Serializer::write(isAI, stream);
	Serializer::write(repairsTotal, stream);
	Serializer::write(repairsLeft, stream);
	Serializer::write(refuelTotal, stream);
	Serializer::write(refuelLeft, stream);
	Serializer::writeVector(activeCars, stream);
}
