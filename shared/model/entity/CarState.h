#pragma once
#include <ISerializable.h>
#include <vector>
#include <Point.h>
#include <BaseEntities.h>
#include <CarProto.h>

class CarState
{
public:
	CarState() = default;
	~CarState() = default;

	Object object;
	Unit unit;
	std::vector<Point> route;
	int16_t routeIndex;
	float progress;
	bool isFinished;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};