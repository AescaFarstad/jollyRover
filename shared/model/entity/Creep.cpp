#include <Creep.h>


void Creep::deserialize(SerializationStream &stream)
{
	Serializer::read(object, stream);
	Serializer::read(unit, stream);
	Serializer::read(weapon, stream);
}

void Creep::serialize(SerializationStream &stream) const
{
	Serializer::write(object, stream);
	Serializer::write(unit, stream);
	Serializer::write(weapon, stream);
}