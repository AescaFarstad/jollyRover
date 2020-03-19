#pragma once
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
	
	const Point& getLocation();

private:

};

#include <Serialization.h>

namespace Serialization
{
	
	//CarState-------------------------------------------------------
	
	template <typename T>
	void write(const CarState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, unit);
		WRITE_FIELD(object, serializer, route);
		WRITE_FIELD(object, serializer, routeIndex);
		WRITE_FIELD(object, serializer, progress);
		WRITE_FIELD(object, serializer, isFinished);
		WRITE_FIELD(object, serializer, startStamp);
		WRITE_FIELD(object, serializer, score);
		WRITE_FIELD(object, serializer, speed);
		WRITE_FIELD(object, serializer, accel);
		WRITE_FIELD(object, serializer, timeSinceRollover);
		WRITE_FIELD(object, serializer, agro);
	}
	
	template <typename T>
	void read(CarState& object, T& serializer)
	{
		READ__FIELD(object, serializer, unit);
		READ__FIELD(object, serializer, route);
		READ__FIELD(object, serializer, routeIndex);
		READ__FIELD(object, serializer, progress);
		READ__FIELD(object, serializer, isFinished);
		READ__FIELD(object, serializer, startStamp);
		READ__FIELD(object, serializer, score);
		READ__FIELD(object, serializer, speed);
		READ__FIELD(object, serializer, accel);
		READ__FIELD(object, serializer, timeSinceRollover);
		READ__FIELD(object, serializer, agro);
	}
}
