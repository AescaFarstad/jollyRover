#include <TimeState.h>


TimeState::TimeState()
{
	time = 0;
	timeScale =1;
	stepsAtOnce = 1;
	forcedStepsAtOnce = -1;
	forcedTimeScale = -1;
	allowedSteps = -1;
	performedSteps = 0;
}

void TimeState::deserialize(SerializationStream& stream)
{
	Serializer::read(time, stream);
	Serializer::read(timeScale, stream);
	Serializer::read(stepsAtOnce, stream);
	Serializer::read(forcedStepsAtOnce, stream);
	Serializer::read(forcedTimeScale, stream);
	Serializer::read(allowedSteps, stream);
	Serializer::read(performedSteps, stream);
}

void TimeState::serialize(SerializationStream& stream) const
{
	Serializer::write(time, stream);
	Serializer::write(timeScale, stream);
	Serializer::write(stepsAtOnce, stream);
	Serializer::write(forcedStepsAtOnce, stream);
	Serializer::write(forcedTimeScale, stream);
	Serializer::write(allowedSteps, stream);
	Serializer::write(performedSteps, stream);
}