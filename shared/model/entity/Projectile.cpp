#include <Projectile.h>

void Projectile::deserialize(SerializationStream & stream)
{
	Serializer::read(object, stream);
	Serializer::read(damage, stream);
	Serializer::read(splash, stream);
	Serializer::read(speed, stream);
	Serializer::read(target, stream);
	Serializer::read(location, stream);
}

void Projectile::serialize(SerializationStream & stream) const 
{
	Serializer::write(object, stream);
	Serializer::write(damage, stream);
	Serializer::write(splash, stream);
	Serializer::write(speed, stream);
	Serializer::write(target, stream);
	Serializer::write(location, stream);
}