#pragma once
#include <PlayerState.h>
#include <TimeState.h>
#include <SeededRandom.h>
#include <CreepState.h>
#include <ProjectileState.h>
#include <FormationState.h>
#include <Prototypes.h>
#include <SpatialMap.h>
#include <EventLogger.h>
#include <ThreatMap.h>
#include <vector>

class GameState
{
public:
	GameState();
	GameState(uint32_t randomSeed);
	
	static const int32_t MAX_CREEPS = 1500;
	
	std::vector<CreepState> creeps;
	std::vector<ProjectileState> projectiles;
	
	std::vector<PlayerState> players;
	std::vector<FormationState> formations;
	SeededRandom random;
	TimeState time;
	uint32_t timeStamp;
	uint32_t idCounter;
	uint32_t loadCount;
	bool isEventLoggerEnabled;
	EventLogger eventLogger;
	
	std::vector<int32_t> forceStrength_;
	SpatialMap<CreepState> creepMap_;
	SpatialMap<CarState> carMap_;
	ThreatMap threatMap_[2];
	std::unordered_map<int32_t, CreepState*> creepById_;
	bool isValid_;
	
	void propagatePrototypes(Prototypes* prototypes);

private:

};

class RoutePoint
{
public:
	Point location;
	bool isValid_;
};

#include <Serialization.h>

namespace Serialization
{
	
	//GameState-------------------------------------------------------
	
	template <typename T>
	void write(const GameState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, timeStamp);
		WRITE_FIELD(object, serializer, time);
		WRITE_FIELD(object, serializer, random);
		WRITE_FIELD(object, serializer, idCounter);
		WRITE_FIELD(object, serializer, loadCount);
		WRITE_FIELD(object, serializer, creeps);
		WRITE_FIELD(object, serializer, projectiles);
		WRITE_FIELD(object, serializer, players);
		WRITE_FIELD(object, serializer, formations);
		WRITE_FIELD(object, serializer, isEventLoggerEnabled);
		if (object.isEventLoggerEnabled)
			WRITE_FIELD(object, serializer, eventLogger);
	}
	
	template <typename T>
	void read(GameState& object, T& serializer)
	{
		READ__FIELD(object, serializer, timeStamp);
		READ__FIELD(object, serializer, time);
		READ__FIELD(object, serializer, random);
		READ__FIELD(object, serializer, idCounter);
		READ__FIELD(object, serializer, loadCount);
		READ__FIELD(object, serializer, creeps);
		READ__FIELD(object, serializer, projectiles);
		READ__FIELD(object, serializer, players);
		READ__FIELD(object, serializer, formations);
		READ__FIELD(object, serializer, isEventLoggerEnabled);
		if (object.isEventLoggerEnabled)
			READ__FIELD(object, serializer, eventLogger);		
		for(auto& creep : object.creeps)
			object.creepById_[creep.unit.id] = &creep;
	}
	
	
	//RoutePoint-------------------------------------------------------
	
	template <typename T>
	void write(const RoutePoint& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, location);
	}
	
	template <typename T>
	void read(RoutePoint& object, T& serializer)
	{
		READ__FIELD(object, serializer, location);
	}
}