#include <CarState.h>


void CarState::deserialize(SerializationStream &stream)
{
	Serializer::read(object, stream);
	Serializer::read(unit, stream);
	Serializer::read(routeIndex, stream);
	Serializer::read(progress, stream);
	Serializer::read(isFinished, stream);
	Serializer::read(score, stream);
	Serializer::read(speed, stream);
	Serializer::read(accel, stream);
	Serializer::readVector(route, stream);
}

void CarState::serialize(SerializationStream &stream) const
{
	Serializer::write(object, stream);
	Serializer::write(unit, stream);
	Serializer::write(routeIndex, stream);
	Serializer::write(progress, stream);
	Serializer::write(isFinished, stream);
	Serializer::write(score, stream);
	Serializer::write(speed, stream);
	Serializer::write(accel, stream);
	Serializer::writeVector(route, stream);
}

const Point& CarState::getLocation()
{
	return unit.location;
}
