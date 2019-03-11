#include <ISerializable.h>


ISerializable::~ISerializable()
{
}
/*
ISerializable::ISerializable()
{
}



void ISerializable::deserialize(SerializationStream & stream)
{
}

void ISerializable::serialize(SerializationStream & stream)
{
}*/


void Serializer::write(ISerializable &value, SerializationStream &stream)
{
	value.serialize(stream);
}

void Serializer::read(ISerializable &value, SerializationStream &stream)
{
	value.deserialize(stream);
}

void Serializer::write(ISerializable* value, SerializationStream &stream)
{
	value->serialize(stream);
}

void Serializer::read(ISerializable* value, SerializationStream &stream)
{
	value->deserialize(stream);
}

