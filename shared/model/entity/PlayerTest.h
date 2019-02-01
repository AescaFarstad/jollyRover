#pragma once
#include <ISerializable.h>
#include <CarRide.h>
class PlayerTest :
	public ISerializable
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

	std::vector<CarRide> activeCars;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
};

