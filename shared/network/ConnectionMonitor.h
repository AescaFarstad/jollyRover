#pragma once

#include <cstdint>

class ConnectionMonitor
{
public:
	void begin(int32_t time, int32_t interval, int32_t timeout);
	bool pollHeartbeat(int32_t time);
	bool isDisconnected(int32_t time);
	void onHearbeatMessage(int32_t time);
	
private:
	int32_t m_lastRcvHBStamp = 0;
	int32_t m_lastSentHBStamp = 0;
	int32_t m_interval = 0;
	int32_t m_timeout = 0;
};