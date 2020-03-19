#include <EventLogger.h>


ProjectileExplosionEvent::ProjectileExplosionEvent(int32_t id, int32_t stamp, int32_t projectileId, int16_t prototypeId, Point location)
{
	this->stamp = stamp;
	this->id = id;
	this->projectileId = projectileId;
	this->prototypeId = prototypeId;
	this->location = location;
}

int32_t ProjectileExplosionEvent::getId()
{
	return id;
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

int32_t CarDeathEvent::getId()
{
	return id;
}

EventLogger::EventLogger()
{
	idCounter = 10;
}