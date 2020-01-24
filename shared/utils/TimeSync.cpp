#include <TimeSync.h>

TimeSync::TimeSync()
{
	m_measurementCount = 0;
	m_minDelta = INT32_MAX;
	m_maxDelta = INT32_MIN;
}

void TimeSync::addMeasurement(uint32_t localRequest, uint32_t serverResponse, uint32_t localResponse)
{
	m_measurementCount++;
	m_minDelta = std::min(m_minDelta, (int64_t)serverResponse - localRequest);
	m_maxDelta = std::max(m_maxDelta, (int64_t)serverResponse - localResponse);	
}

int64_t TimeSync::localToServerUpperBound(uint32_t local)
{
	return local + m_minDelta;
}

int64_t TimeSync::localToServerLowerBound(uint32_t local)
{
	return local + m_maxDelta;
}

int64_t TimeSync::serverToLocalUpperBound(uint32_t server)
{
	return server - m_maxDelta;
}

int64_t TimeSync::serverToLocalLowerBound(uint32_t server)
{
	return server - m_minDelta;
}

int32_t TimeSync::getUncertainty()
{
	return (int32_t)(m_minDelta - m_maxDelta);
}
