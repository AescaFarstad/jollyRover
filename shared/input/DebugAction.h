#pragma once
#include <cstdint>


enum class DEBUG_ACTION : int8_t
{
	NONE,
	BOOM,
	KILL_LEFT,
	KILL_RIGHT,
	RETREAT
};

#include <Serialization.h>

namespace Serialization
{
	
	//DEBUG_ACTION---------------------------------------------------
	
	template <typename T>
	void write(const DEBUG_ACTION& object, T& serializer)
	{
		serializer.write((int8_t)object, FIELD_NAME(DEBUG_ACTION));
	}
	
	template <typename T>
	void read(DEBUG_ACTION& object, T& serializer)
	{
		int8_t tmp;
		serializer.read(tmp, FIELD_NAME(DEBUG_ACTION));
		object = (DEBUG_ACTION)tmp;
	}
}