#pragma once
#include <PlayerTest.h>
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
/*
class ForceState
{
	ForceState() = default;
	~ForceState() = default;
	
	
	
};*/

class GameState
{
public:
	GameState();
	GameState(uint32_t randomSeed);
	~GameState() = default;
	
	static const int32_t MAX_CREEPS = 1500;
	
	std::vector<CreepState> creeps;
	std::vector<Projectile> projectiles;
	//std::vector<ForceState> forces;
	
	std::vector<PlayerTest> players;
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