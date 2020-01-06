#include <SimplePacket.h>

void SimplePacket::setPayloadFromRawData(const char* newRawData, size_t size)
{
	payload = new char[size]();
	memcpy(payload, newRawData, size);
	payloadSize = (int16_t)size;

	rawData = payload;
	rawSize = payloadSize;
}
