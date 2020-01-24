#pragma once
#include <PlayerState.h>
#include <TimeState.h>
#include <SeededRandom.h>
#include <ISerializable.h>
#include <CreepState.h>
#include <Projectile.h>
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
	std::vector<Projectile> projectiles;
	
	std::vector<PlayerState> players;
	std::vector<FormationState> formations;
	SeededRandom random;
	TimeState time;
	uint32_t timeStamp;
	uint32_t idCounter;
	uint32_t loadCount;
	bool isEventLoggerEnabled;
	EventLogger eventLogger;
	
	std::vector<int> forceStrength_;
	SpatialMap<CreepState> creepMap_;
	SpatialMap<CarState> carMap_;
	ThreatMap threatMap_[2];
	std::unordered_map<int32_t, CreepState*> creepById_;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	void propagatePrototypes(Prototypes* prototypes);

private:

};

class RoutePoint
{
public:
	Point location;
	bool isValid_;
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};