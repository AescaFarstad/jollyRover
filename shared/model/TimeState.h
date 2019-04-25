#pragma once
#include <ISerializable.h>

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
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};
