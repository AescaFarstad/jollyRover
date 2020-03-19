#pragma once
#include <cstdint>

class TimeState
{
public:
	TimeState();
	
	int32_t time;
	float timeScale;
	int32_t stepsAtOnce;
	int32_t forcedStepsAtOnce;
	float forcedTimeScale; //ignores allowedSteps and overrides timeScale if (forcedTimeScale != 0)
	int32_t allowedSteps;
	int32_t performedSteps;
};

#include <Serialization.h>

namespace Serialization
{
	
	//TimeState-------------------------------------------------------
	
	template <typename T>
	void write(const TimeState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, time);
		WRITE_FIELD(object, serializer, timeScale);
		WRITE_FIELD(object, serializer, stepsAtOnce);
		WRITE_FIELD(object, serializer, forcedStepsAtOnce);
		WRITE_FIELD(object, serializer, forcedTimeScale);
		WRITE_FIELD(object, serializer, allowedSteps);
		WRITE_FIELD(object, serializer, performedSteps);
	}
	
	template <typename T>
	void read(TimeState& object, T& serializer)
	{		
		READ__FIELD(object, serializer, time);
		READ__FIELD(object, serializer, timeScale);
		READ__FIELD(object, serializer, stepsAtOnce);
		READ__FIELD(object, serializer, forcedStepsAtOnce);
		READ__FIELD(object, serializer, forcedTimeScale);
		READ__FIELD(object, serializer, allowedSteps);
		READ__FIELD(object, serializer, performedSteps);
	}
}