#include <TimeSync.h>



TimeSync::TimeSync()
{
	measurementCount = 0;
	minDelta = INT32_MAX;
	maxDelta = -INT32_MIN;
}


TimeSync::~TimeSync()
{
}

void TimeSync::addMeasurement(uint32_t localRequest, uint32_t serverResponse, uint32_t localResponse)
{
	measurementCount++;
	minDelta = std::min(minDelta, (int64_t)serverResponse - localRequest);
	maxDelta = std::max(maxDelta, (int64_t)serverResponse - localResponse);	
}

int64_t TimeSync::localToServerUpperBound(uint32_t local)
{
	return local + minDelta;
}

int64_t TimeSync::localToServerLowerBound(uint32_t local)
{
	return local + maxDelta;
}

int64_t TimeSync::serverToLocalUpperBound(uint32_t server)
{
	return server - maxDelta;
}

int64_t TimeSync::serverToLocalLowerBound(uint32_t server)
{
	return server - minDelta;
}

int32_t TimeSync::getUncertainty()
{
	return (int32_t)(minDelta - maxDelta);
}
