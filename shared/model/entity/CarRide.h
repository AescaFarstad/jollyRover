#pragma once
#include <ISerializable.h>
#include <vector>
#include <Point.h>
#include <Car.h>
#include <CarProto.h>

class CarRide : public ISerializable
{
public:
	CarRide();
	CarRide(std::vector<Point> route, CarProto &car);
	~CarRide();

	Car car;
	std::vector<Point> route;
	int16_t routeIndex;
	float progress;
	bool isFinished;

	int ui;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) override;

private:

};
