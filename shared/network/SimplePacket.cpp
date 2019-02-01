#include <SimplePacket.h>

SimplePacket::SimplePacket()
{
}

SimplePacket::~SimplePacket()
{
}

void SimplePacket::setPayloadFromRawData(const char * newRawData, size_t size)
{
	payload = new char[size]();
	memcpy(payload, newRawData, size);
	payloadSize = (int16_t)size;

	rawData = payload;
	rawSize = payloadSize;
}

void SimplePacket::loadFromRawData(const char * rawData, int bytesAvailable)
{
	THROW_FATAL_ERROR("Not Implemented");
}
