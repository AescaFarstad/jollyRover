#pragma once
#include <ISerializable.h>
#include <CarState.h>

class PlayerTest
{
public:

	int32_t login;

	std::vector<CarState> activeCars;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

