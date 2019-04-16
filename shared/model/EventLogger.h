#pragma once
#include <Point.h>
#include <CircularContainer.h>

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
	UnitDeathEvent() = default;
	UnitDeathEvent(int32_t id, int32_t stamp, int32_t unitId, int16_t prototypeId, Point location);
	
	int32_t stamp;
	int32_t id;
	int32_t unitId;
	int16_t prototypeId;
	Point location;
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};



class EventLogger
{
public:
	EventLogger();
	
	int32_t idCounter;
	CircularContainer<ProjectileExplosionEvent, 501> projectileExplosions;
	CircularContainer<UnitDeathEvent, 51> unitDeaths;
	
	template <class... Args>
	void addProjectileExplosion(Args&&... args)
	{
		projectileExplosions.add(idCounter++, std::forward<Args>(args)...);
	}
	
	template <class... Args>
	void addUnitDeath(Args&&... args)
	{
		unitDeaths.add(idCounter++, std::forward<Args>(args)...);
	}
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	
};



