#include <DebugAction.h>

void Serializer::write(const DEBUG_ACTION& value, SerializationStream& stream)
{
	Serializer::write((int8_t)value, stream);
}

void Serializer::read(DEBUG_ACTION& value, SerializationStream& stream)
{
	int8_t t;
	Serializer::read(t, stream);
	value = (DEBUG_ACTION)t;
}