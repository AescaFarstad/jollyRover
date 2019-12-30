#include <Projectile.h>

void Projectile::deserialize(SerializationStream & stream)
{
	Serializer::read(id, stream);
	Serializer::read(prototypeId, stream);
	Serializer::read(damage, stream);
	Serializer::read(force, stream);
	Serializer::read(splash, stream);
	Serializer::read(speed, stream);
	Serializer::read(weapon, stream);
	Serializer::read(spawnedAt, stream);
	Serializer::read(target, stream);
	Serializer::read(location, stream);
}

void Projectile::serialize(SerializationStream & stream) const 
{
	Serializer::write(id, stream);
	Serializer::write(prototypeId, stream);
	Serializer::write(damage, stream);
	Serializer::write(force, stream);
	Serializer::write(splash, stream);
	Serializer::write(speed, stream);
	Serializer::write(weapon, stream);
	Serializer::write(spawnedAt, stream);
	Serializer::write(target, stream);
	Serializer::write(location, stream);
}