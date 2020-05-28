#pragma once
#include <Serialization.h>
enum class DEMO_EVENT : int8_t {
	STATE = 1,
	INPUT = 2
};

namespace Serialization
{
	//DEMO_EVENT-------------------------------------------------------
	
	template <typename T>
	void write(const DEMO_EVENT& object, T& serializer)
	{
		serializer.write((int8_t)object, FIELD_NAME(DEMO_EVENT));
	}
	
	template <typename T>
	void read(DEMO_EVENT& object, T& serializer)
	{
		int8_t tmp;
		serializer.read(tmp, FIELD_NAME(DEMO_EVENT));
		object = (DEMO_EVENT)tmp;
	}
}