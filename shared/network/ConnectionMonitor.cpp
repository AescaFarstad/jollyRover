#include <ConnectionMonitor.h>

void ConnectionMonitor::begin(int32_t time, int32_t interval, int32_t timeout)
{
	m_lastRcvHBStamp = time;
	m_lastSentHBStamp = time;
	m_interval = interval;
	m_timeout = timeout;
}

bool ConnectionMonitor::pollHeartbeat(int32_t time)
{
	bool result = time - m_lastSentHBStamp > m_interval;
	if (result)
		m_lastSentHBStamp = time;
	return result;
}

bool ConnectionMonitor::isDisconnected(int32_t time)
{
	return time - m_lastRcvHBStamp > m_timeout;
}

void ConnectionMonitor::onHearbeatMessage(int32_t time)
{
	m_lastRcvHBStamp = time;
}