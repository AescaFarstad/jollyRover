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