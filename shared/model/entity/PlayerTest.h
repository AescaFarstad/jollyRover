#pragma once
#include <ISerializable.h>
#include <CarState.h>
class PlayerTest
{
public:
	PlayerTest();
	~PlayerTest();
	float x;
	float y;
	float speedX;
	float speedY;

	bool buttonDown_Left;
	bool buttonDown_Right;
	bool buttonDown_Forward;
	bool buttonDown_Backward;

	int32_t login;

	std::vector<CarState> activeCars;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};

