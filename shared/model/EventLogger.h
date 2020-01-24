#pragma once
#include <Point.h>
#include <CircularContainer.h>
#include <CreepState.h>
#include <CarState.h>

class ProjectileExplosionEvent
{
public:
	ProjectileExplosionEvent() = default;
	ProjectileExplosionEvent(int32_t id, int32_t stamp, int32_t projectileId, int16_t prototypeId, Point location);
	
	int32_t stamp;
	int32_t id;
	int32_t projectileId;
	int16_t prototypeId;
	Point location;	
	
	int32_t getId();
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

class UnitDeathEvent
{
public:
	int32_t unitId;
	int16_t prototypeId;
	Point location;
	float rotation;
	Point impact;
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

class CreepDeathEvent
{
public:
	CreepDeathEvent() = default;
	CreepDeathEvent(int32_t id, int32_t stamp, const CreepState& creep, Point impact);
	
	int32_t stamp;
	int32_t id;
	int16_t force;
	UnitDeathEvent unitDeath;
	
	int32_t getId();
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

class CarDeathEvent
{
public:
	CarDeathEvent() = default;
	CarDeathEvent(int32_t id, int32_t stamp, const CarState& car, int32_t player, Point impact);	
	
	int32_t stamp;
	int32_t id;
	int32_t player;
	UnitDeathEvent unitDeath;
	
	int32_t getId();
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};



class EventLogger
{
public:
	EventLogger();
	
	int32_t idCounter;
	CircularContainer<ProjectileExplosionEvent, 501> projectileExplosions;
	CircularContainer<CreepDeathEvent, 51> creepDeaths;
	CircularContainer<CarDeathEvent, 10> carDeaths;
	
	void addProjectileExplosion(int32_t stamp, int32_t id, int32_t prototypeId, const Point& target)
	{
		projectileExplosions.add(idCounter++, stamp, id, prototypeId, target);
	}
	
	void addCreepDeath(int32_t stamp, const CreepState& creep, const Point& impact)
	{
		creepDeaths.add(idCounter++, stamp, creep, impact);
	}
	
	void addCarDeath(int32_t stamp, const CarState& car, int32_t player, const Point& impact)
	{
		carDeaths.add(idCounter++, stamp, car, player, impact);
	}
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	
};



