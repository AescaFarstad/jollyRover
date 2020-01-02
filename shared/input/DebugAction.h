#pragma once
#include <cstdint>
#include <ISerializable.h>


enum class DEBUG_ACTION : int8_t
{
	NONE,
	BOOM,
	KILL_LEFT,
	KILL_RIGHT,
	RETREAT
};

namespace Serializer {

	void write(const DEBUG_ACTION& value, SerializationStream& stream);
	void read(DEBUG_ACTION& value, SerializationStream& stream);
}