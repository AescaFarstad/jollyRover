#pragma once
#include <cstdint>

enum class INPUT_IMPULSE : int8_t {
	NONE,
	ADD_AI,
	CLEAR_AI,
	ACT_AI,
	TOGGLE_AI,
	TMP_DEBUG
	
};

#include <Serialization.h>

namespace Serialization
{
	
	//INPUT_IMPULSE---------------------------------------------------
	
	template <typename T>
	void write(const INPUT_IMPULSE& object, T& serializer)
	{
		serializer.write((int8_t)object, FIELD_NAME(INPUT_IMPULSE));
	}
	
	template <typename T>
	void read(INPUT_IMPULSE& object, T& serializer)
	{
		int8_t tmp;
		serializer.read(tmp, FIELD_NAME(INPUT_IMPULSE));
		object = (INPUT_IMPULSE)tmp;
	}
}