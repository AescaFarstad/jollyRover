#pragma once
#include <cstdint>
#include <ISerializable.h>

enum class INPUT_IMPULSE : int8_t {
	NONE,
	ADD_AI,
	CLEAR_AI,
	ACT_AI
	
};


namespace Serializer {

	void write(const INPUT_IMPULSE& value, SerializationStream& stream);
	void read(INPUT_IMPULSE& value, SerializationStream& stream);
}