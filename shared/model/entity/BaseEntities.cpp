#include <BaseEntities.h>

void Unit::deserialize(SerializationStream &stream)
{
	Serializer::read(id, stream);
	Serializer::read(prototypeId, stream);
	Serializer::read(force, stream);
	Serializer::read(health, stream);
	Serializer::read(location, stream);
	Serializer::read(voluntaryMovement, stream);
}

void Unit::serialize(SerializationStream &stream) const
{
	Serializer::write(id, stream);
	Serializer::write(prototypeId, stream);
	Serializer::write(force, stream);
	Serializer::write(health, stream);
	Serializer::write(location, stream);
	Serializer::write(voluntaryMovement, stream);
}

//-----------------------------------------------------------------------
void Target::deserialize(SerializationStream &stream)
{
	Serializer::read(id, stream);
	Serializer::read(type, stream);
}

void Target::serialize(SerializationStream &stream) const
{
	Serializer::write(id, stream);
	Serializer::write(type, stream);
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

//-----------------------------------------------------------------------
void Serializer::write(const ENTTITY_TYPE& value, SerializationStream& stream)
{
	Serializer::write((int8_t)value, stream);
}

void Serializer::read(ENTTITY_TYPE& value, SerializationStream& stream)
{
	int8_t t;
	Serializer::read(t, stream);
	value = (ENTTITY_TYPE)t;
}