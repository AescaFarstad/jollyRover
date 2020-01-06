#pragma once
#include <NetworkPacket.h>

class SimplePacket : public NetworkPacket
{
public:
	SimplePacket() = default;
	virtual ~SimplePacket() = default;

	virtual void setPayloadFromRawData(const char* newRawData, size_t size);
};