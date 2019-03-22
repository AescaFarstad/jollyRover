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


void Serializer::write(MessageTypes& value, SerializationStream &stream)
{
	Serializer::write((std::underlying_type<MessageTypes>::type)value, stream);
}

void Serializer::read(MessageTypes& value, SerializationStream &stream)
{
	std::underlying_type<MessageTypes>::type tmp;
	Serializer::read(tmp, stream);
	value = (MessageTypes)tmp;
}
void Serializer::write(RequestTypes& value, SerializationStream &stream)
{
	Serializer::write((std::underlying_type<RequestTypes>::type)value, stream);
}

void Serializer::read(RequestTypes& value, SerializationStream &stream)
{	
	std::underlying_type<RequestTypes>::type tmp;
	Serializer::read(tmp, stream);
	value = (RequestTypes)tmp;
}

