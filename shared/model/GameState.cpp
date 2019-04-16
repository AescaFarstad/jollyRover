#include <GameState.h>

GameState::GameState()
{
	timeStamp = 0;
	idCounter = 1;
}

GameState::GameState(uint32_t randomSeed) : GameState()
{
	random.initFromSeed(randomSeed);
}


void GameState::deserialize(SerializationStream& stream)
{
	Serializer::read(timeStamp, stream);
	Serializer::read(random, stream);
	Serializer::read(time, stream);
	Serializer::read(idCounter, stream);
	Serializer::read(isEventLoggerEnabled, stream);
	if (isEventLoggerEnabled)
		Serializer::read(eventLogger, stream);
	Serializer::readVector(players, stream);
	Serializer::readVector(creeps, stream);
	Serializer::readVector(projectiles, stream);
	Serializer::readVector(formations, stream);
}

void GameState::propagatePrototypes(Prototypes* prototypes)
{
	for(auto& creep : creeps)
	{
		creep.propagatePrototypes(prototypes->creeps, prototypes->weapons);
	}
	for(auto& formation : formations)
	{
		formation.propagatePrototypes(prototypes->formations, prototypes->objectives);
	}
}

void GameState::serialize(SerializationStream& stream) const 
{
	Serializer::write(timeStamp, stream);
	Serializer::write(random, stream);
	Serializer::write(time, stream);
	Serializer::write(idCounter, stream);
	Serializer::write(isEventLoggerEnabled, stream);
	if (isEventLoggerEnabled)
		Serializer::write(eventLogger, stream);
	Serializer::writeVector(players, stream);
	Serializer::writeVector(creeps, stream);
	Serializer::writeVector(projectiles, stream);
	Serializer::writeVector(formations, stream);
}

void RoutePoint::deserialize(SerializationStream& stream)
{
	Serializer::read(location, stream);
}

void RoutePoint::serialize(SerializationStream& stream) const 
{
	Serializer::write(location, stream);
}
