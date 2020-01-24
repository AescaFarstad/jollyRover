#pragma once
#include <ISerializable.h>
#include <vector>
#include <Point.h>
#include <BaseEntities.h>
#include <CarProto.h>

class CarState
{
public:
	CarState();
	
	Unit unit;
	std::vector<Point> route;
	int16_t routeIndex;
	float progress;
	bool isFinished;
	int32_t startStamp;
	int32_t score;
	float speed;
	float accel;
	int32_t timeSinceRollover;
	int32_t agro;
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	
	const Point& getLocation();

private:

};
