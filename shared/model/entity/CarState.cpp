#include <CarState.h>

CarState::CarState()
{
	unit.host_.type = ENTTITY_TYPE::CAR;
	unit.host_.pointer = this;
}
	
void CarState::deserialize(SerializationStream &stream)
{
	Serializer::read(unit, stream);
	Serializer::read(routeIndex, stream);
	Serializer::read(progress, stream);
	Serializer::read(isFinished, stream);
	Serializer::read(startStamp, stream);
	Serializer::read(score, stream);
	Serializer::read(speed, stream);
	Serializer::read(accel, stream);
	Serializer::read(timeSinceRollover, stream);
	Serializer::read(agro, stream);
	Serializer::readVector(route, stream);
}

void CarState::serialize(SerializationStream &stream) const
{
	Serializer::write(unit, stream);
	Serializer::write(routeIndex, stream);
	Serializer::write(progress, stream);
	Serializer::write(isFinished, stream);
	Serializer::write(startStamp, stream);
	Serializer::write(score, stream);
	Serializer::write(speed, stream);
	Serializer::write(accel, stream);
	Serializer::write(timeSinceRollover, stream);
	Serializer::write(agro, stream);
	Serializer::writeVector(route, stream);
}

const Point& CarState::getLocation()
{
	return unit.location;
}
