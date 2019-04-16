#include <EventLogger.h>


ProjectileExplosionEvent::ProjectileExplosionEvent(int32_t id, int32_t stamp, int32_t projectileId, int16_t prototypeId, Point location)
{
	this->stamp = stamp;
	this->id = id;
	this->projectileId = projectileId;
	this->prototypeId = prototypeId;
	this->location = location;
}
	
void ProjectileExplosionEvent::deserialize(SerializationStream& stream)
{
	Serializer::read(stamp, stream);
	Serializer::read(id, stream);
	Serializer::read(projectileId, stream);
	Serializer::read(prototypeId, stream);
	Serializer::read(location, stream);
}

int32_t ProjectileExplosionEvent::getId()
{
	return id;
}

void ProjectileExplosionEvent::serialize(SerializationStream& stream) const 
{
	Serializer::write(stamp, stream);
	Serializer::write(id, stream);
	Serializer::write(projectileId, stream);
	Serializer::write(prototypeId, stream);
	Serializer::write(location, stream);
}

UnitDeathEvent::UnitDeathEvent(int32_t id, int32_t stamp, int32_t unitId, int16_t prototypeId, Point location)
{
	this->stamp = stamp;
	this->id = id;
	this->unitId = unitId;
	this->prototypeId = prototypeId;
	this->location = location;
	
}
	
void UnitDeathEvent::deserialize(SerializationStream& stream)
{
	Serializer::read(stamp, stream);
	Serializer::read(id, stream);
	Serializer::read(unitId, stream);
	Serializer::read(prototypeId, stream);
	Serializer::read(location, stream);
}

void UnitDeathEvent::serialize(SerializationStream& stream) const 
{
	Serializer::write(stamp, stream);
	Serializer::write(id, stream);
	Serializer::write(unitId, stream);
	Serializer::write(prototypeId, stream);
	Serializer::write(location, stream);
}

EventLogger::EventLogger()
{
	idCounter = 10;
}

	
void EventLogger::deserialize(SerializationStream& stream)
{
	Serializer::read(idCounter, stream);
}

void EventLogger::serialize(SerializationStream& stream) const 
{
	Serializer::write(idCounter, stream);
}