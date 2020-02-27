#include <PlayerState.h>


PlayerState::PlayerState()
{
	login = 0; 
	score = 0;
	isHeadless = false;
	isAI = false;
	repairsTotal = 0;
	repairsLeft = 0;
	refuelTotal = 0;
	refuelLeft = 0;
	wentOfflineAt = 0;
}

PlayerState::PlayerState(int32_t login) : PlayerState()
{
	this->login = login;
}

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
	Serializer::read(wentOfflineAt, stream);
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
	Serializer::write(wentOfflineAt, stream);
	Serializer::writeVector(activeCars, stream);
}
