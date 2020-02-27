#pragma once
#include <ISerializable.h>
#include <CarState.h>

class PlayerState
{
public:
	PlayerState();
	explicit PlayerState(int32_t login);

	int32_t login;
	int32_t score;
	bool isHeadless;
	bool isAI;
	
	int32_t repairsTotal;
	int32_t repairsLeft;
	int32_t refuelTotal;
	int32_t refuelLeft;
	
	uint32_t wentOfflineAt;

	std::vector<CarState> activeCars;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

