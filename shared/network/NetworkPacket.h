#pragma once
#include <BinarySerializer.h>
#include <NetworkMessage.h>

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
	
	template <typename T>
	void setPayloadFromSerializable(const T& serializable, size_t expectedSize = 2048)
	{
		BinarySerializer bs;
		bs.write(serializable);
		auto data = bs.dumpAll();
		setPayloadFromRawData(&data[0], data.size());
	}
	
	

	virtual void loadFromRawData(const char* rawData, int32_t bytesAvailable = 4096);

	void tracePayload();
	bool isFullyLoaded();

private:
	static const int16_t SUGAR = 1934;
	static const int16_t bufferSize = sizeof(uint16_t) * 2;
	char tempBuffer[bufferSize];

};

template <>
void NetworkPacket::setPayloadFromSerializable<>(const NetworkMessage& serializable, size_t expectedSize);