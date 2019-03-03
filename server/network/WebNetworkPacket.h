#pragma once
#include "NetworkPacket.h"
#include "SystemInfo.h"

class WebNetworkPacket :
	public NetworkPacket
{
public:
	WebNetworkPacket();
	~WebNetworkPacket();

	virtual void setPayloadFromRawData(const char* newRawData, size_t size);

	virtual void loadFromRawData(const char* rawData, int bytesAvailable = 4096);


private:

	static const int16_t SUGAR = 1934;
	static const int16_t bufferSize1 = sizeof(uint8_t) * 2;
	char tempBuffer1[bufferSize1];
	int16_t bufferSize2;
	char* tempBuffer2;
};


