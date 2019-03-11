#include <Car.h>

Car::Car()
{
}

Car::~Car()
{
}

void Car::init(CarProto &car)
{
	health = car.maxHealth;
	prototypeId = car.id;
}

void Car::deserialize(SerializationStream &stream)
{
	Serializer::read(prototypeId, stream);
	Serializer::read(health, stream);
}

void Car::serialize(SerializationStream &stream) const
{
	Serializer::write(prototypeId, stream);
	Serializer::write(health, stream);
}
