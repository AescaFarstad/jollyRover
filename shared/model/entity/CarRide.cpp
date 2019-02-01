#include <CarRide.h>

CarRide::CarRide()
{
	ui = S::getId();
	//printf("ride constructed a, %d", ui);
}

CarRide::CarRide(std::vector<Point> route, CarProto &car)
{
	this->route = route;
	progress = 0;
	routeIndex = 0;
	isFinished = false;
	this->car.init(car);
	ui = S::getId();
	//printf("ride constructed, %d", ui);
}

CarRide::~CarRide()
{
	//printf("ride destructed, %d", ui);
}

void CarRide::deserialize(SerializationStream &stream)
{
	Serializer::read(car, stream);
	Serializer::read(routeIndex, stream);
	Serializer::read(progress, stream);
	Serializer::read(isFinished, stream);
	Serializer::readVector(route, stream);
}

void CarRide::serialize(SerializationStream &stream)
{
	Serializer::write(car, stream);
	Serializer::write(routeIndex, stream);
	Serializer::write(progress, stream);
	Serializer::write(isFinished, stream);
	Serializer::writeVector(route, stream);
}
