#pragma once
#include <NetworkPacket.h>

class SimplePacket : public NetworkPacket
{
public:
	SimplePacket();
	~SimplePacket();

	virtual void setPayloadFromRawData(const char* newRawData, size_t size);
	virtual void loadFromRawData(const char* rawData, int bytesAvailable = 4096);
};