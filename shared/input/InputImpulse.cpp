#include <InputImpulse.h>

void Serializer::write(const INPUT_IMPULSE& value, SerializationStream& stream)
{
	Serializer::write((int8_t)value, stream);
}

void Serializer::read(INPUT_IMPULSE& value, SerializationStream& stream)
{
	int8_t t;
	Serializer::read(t, stream);
	value = (INPUT_IMPULSE)t;
}