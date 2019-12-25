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


void Serializer::write(const MESSAGE_TYPE& value, SerializationStream &stream)
{
	Serializer::write((std::underlying_type<MESSAGE_TYPE>::type)value, stream);
}

void Serializer::read(MESSAGE_TYPE& value, SerializationStream &stream)
{
	std::underlying_type<MESSAGE_TYPE>::type tmp;
	Serializer::read(tmp, stream);
	value = (MESSAGE_TYPE)tmp;
}
void Serializer::write(const REQUEST_TYPE& value, SerializationStream &stream)
{
	Serializer::write((std::underlying_type<REQUEST_TYPE>::type)value, stream);
}

void Serializer::read(REQUEST_TYPE& value, SerializationStream &stream)
{	
	std::underlying_type<REQUEST_TYPE>::type tmp;
	Serializer::read(tmp, stream);
	value = (REQUEST_TYPE)tmp;
}

