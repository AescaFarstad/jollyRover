#pragma once
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
};

#include <Serialization.h>

namespace Serialization
{
	
	//PlayerState-------------------------------------------------------
	
	template <typename T>
	void write(const PlayerState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, login);
		WRITE_FIELD(object, serializer, score);
		WRITE_FIELD(object, serializer, isHeadless);
		WRITE_FIELD(object, serializer, isAI);
		WRITE_FIELD(object, serializer, repairsTotal);
		WRITE_FIELD(object, serializer, repairsLeft);
		WRITE_FIELD(object, serializer, refuelTotal);
		WRITE_FIELD(object, serializer, refuelLeft);
		WRITE_FIELD(object, serializer, wentOfflineAt);
		WRITE_FIELD(object, serializer, activeCars);
	}
	
	template <typename T>
	void read(PlayerState& object, T& serializer)
	{
		READ__FIELD(object, serializer, login);
		READ__FIELD(object, serializer, score);
		READ__FIELD(object, serializer, isHeadless);
		READ__FIELD(object, serializer, isAI);
		READ__FIELD(object, serializer, repairsTotal);
		READ__FIELD(object, serializer, repairsLeft);
		READ__FIELD(object, serializer, refuelTotal);
		READ__FIELD(object, serializer, refuelLeft);
		READ__FIELD(object, serializer, wentOfflineAt);
		READ__FIELD(object, serializer, activeCars);
	}
}