#include <InputImpulse.h>

void Serializer::write(const INPUT_IMPULSE& value, SerializationStream& stream)
{
	Serializer::write((int32_t)value, stream);
}

void Serializer::read(INPUT_IMPULSE& value, SerializationStream& stream)
{
	int32_t t;
	Serializer::read(t, stream);
	value = (INPUT_IMPULSE)t;
}