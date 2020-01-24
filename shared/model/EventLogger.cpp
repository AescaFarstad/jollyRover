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

void UnitDeathEvent::deserialize(SerializationStream& stream)
{
	Serializer::read(unitId, stream);
	Serializer::read(prototypeId, stream);
	Serializer::read(location, stream);
	Serializer::read(rotation, stream);
	Serializer::read(impact, stream);
}

void UnitDeathEvent::serialize(SerializationStream& stream) const 
{
	Serializer::write(unitId, stream);
	Serializer::write(prototypeId, stream);
	Serializer::write(location, stream);
	Serializer::write(rotation, stream);
	Serializer::write(impact, stream);
}

CreepDeathEvent::CreepDeathEvent(int32_t id, int32_t stamp, const CreepState& creep, Point impact)
{
	this->stamp = stamp;
	this->id = id;
	this->unitDeath.unitId = creep.unit.id;
	this->force = creep.unit.force;
	this->unitDeath.prototypeId = creep.unit.prototypeId;
	this->unitDeath.location = creep.unit.location;
	this->unitDeath.rotation = creep.unit.voluntaryMovement.asAngle();
	this->unitDeath.impact = impact;	
}
	
void CreepDeathEvent::deserialize(SerializationStream& stream)
{
	Serializer::read(stamp, stream);
	Serializer::read(id, stream);
	Serializer::read(force, stream);
	Serializer::read(unitDeath, stream);
}

void CreepDeathEvent::serialize(SerializationStream& stream) const 
{
	Serializer::write(stamp, stream);
	Serializer::write(id, stream);
	Serializer::write(force, stream);
	Serializer::write(unitDeath, stream);
}

int32_t CreepDeathEvent::getId()
{
	return id;
}

CarDeathEvent::CarDeathEvent(int32_t id, int32_t stamp, const CarState& car, int32_t player, Point impact)
{
	this->stamp = stamp;
	this->id = id;
	this->unitDeath.unitId = car.unit.id;
	this->player = player;
	this->unitDeath.prototypeId = car.unit.prototypeId;
	this->unitDeath.location = car.unit.location;
	this->unitDeath.rotation = car.unit.voluntaryMovement.asAngle();
	this->unitDeath.impact = impact;	
}
	
void CarDeathEvent::deserialize(SerializationStream& stream)
{
	Serializer::read(stamp, stream);
	Serializer::read(id, stream);
	Serializer::read(player, stream);
	Serializer::read(unitDeath, stream);
}

void CarDeathEvent::serialize(SerializationStream& stream) const 
{
	Serializer::write(stamp, stream);
	Serializer::write(id, stream);
	Serializer::write(player, stream);
	Serializer::write(unitDeath, stream);
}

int32_t CarDeathEvent::getId()
{
	return id;
}

EventLogger::EventLogger()
{
	idCounter = 10;
}

	
void EventLogger::deserialize(SerializationStream& stream)
{
	Serializer::read(idCounter, stream);
	//TODO serialize events
}

void EventLogger::serialize(SerializationStream& stream) const 
{
	Serializer::write(idCounter, stream);
	//TODO serialize events
}