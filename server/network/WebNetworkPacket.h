#pragma once
#include <NetworkPacket.h>

class WebNetworkPacket :
	public NetworkPacket
{
public:
	WebNetworkPacket();
	~WebNetworkPacket();

	virtual void setPayloadFromRawData(const char* newRawData, size_t size);

	virtual void loadFromRawData(const char* rawData, int32_t bytesAvailable = 4096);


private:

	static constexpr int16_t CYPHER_KEY_SIZE = 4;
	static constexpr int16_t bufferSize_p1 = sizeof(uint8_t) * 2;
	char headerBuffer_p1[bufferSize_p1];
	int16_t bufferSize_p2;
	char* headerBuffer_p2;
};



