#pragma once
#include <string>
#include <ISerializable.h>
#include <SerializationStream.h>

class NetworkPacket
{
public:
	NetworkPacket();
	virtual ~NetworkPacket();

	int32_t protocol;

	uint16_t rawSize;
	char* rawData;

	uint16_t payloadSize;
	char* payload;

	uint16_t bytesLoaded;
	bool isDisconnectNotice;

	virtual void setPayloadFromString(std::string str);
	virtual void setPayloadFromRawData(const char* newRawData, size_t size);
	virtual void setPayloadFromSerializable(const ISerializable& serializable, size_t expectedSize = 2048);

	virtual void loadFromRawData(const char* rawData, int32_t bytesAvailable = 4096);

	void tracePayload();
	bool isFullyLoaded();



private:
	static const int16_t SUGAR = 1934;
	static const int16_t bufferSize = sizeof(uint16_t) * 2;
	char tempBuffer[bufferSize];

};