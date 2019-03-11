#include <BaseEntities.h>


void Object::deserialize(SerializationStream &stream)
{
	Serializer::read(id, stream);
	Serializer::read(prototypeId, stream);
}

void Object::serialize(SerializationStream &stream) const
{
	Serializer::write(id, stream);
	Serializer::write(prototypeId, stream);
}

//-----------------------------------------------------------------------
void Unit::deserialize(SerializationStream &stream)
{
	Serializer::read(force, stream);
	Serializer::read(health, stream);
	Serializer::read(location, stream);
}

void Unit::serialize(SerializationStream &stream) const
{
	Serializer::write(force, stream);
	Serializer::write(health, stream);
	Serializer::write(location, stream);
}


//-----------------------------------------------------------------------
void Weapon::deserialize(SerializationStream &stream)
{
	Serializer::read(prototypeId, stream);
	Serializer::read(attackCooldown, stream);
	Serializer::read(target, stream);
}

void Weapon::serialize(SerializationStream &stream) const
{
	Serializer::write(prototypeId, stream);
	Serializer::write(attackCooldown, stream);
	Serializer::write(target, stream);
}