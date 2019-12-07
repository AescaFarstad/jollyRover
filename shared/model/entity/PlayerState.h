#pragma once
#include <ISerializable.h>
#include <CarState.h>

class PlayerState
{
public:

	int32_t login;
	int32_t score;
	bool isHeadless;
	bool isAI;

	std::vector<CarState> activeCars;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

