#include <NetworkPacket.h>
#include <string>
#include <iostream>
#include <SerializeSimpleTypes.h>
#include <Global.h>

NetworkPacket::NetworkPacket()
{
	payloadSize = 0;
	rawSize = 0;
	bytesLoaded = 0;
	rawData = nullptr;
	payload = nullptr;
	isDisconnectNotice = false;
}

NetworkPacket::~NetworkPacket()
{
	if (rawData != nullptr)
		delete[] rawData;
}

void NetworkPacket::setPayloadFromString(std::string str)
{
	setPayloadFromRawData(str.c_str(), str.length());
}

void NetworkPacket::loadFromRawData(const char* rawData, int32_t bytesAvailable)
{
	if (bytesLoaded < bufferSize)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize - bytesLoaded);
		if (newBytes <= 0)
			printf("trouble: %d, %d, %d", bytesAvailable, bufferSize, bytesLoaded);
		std::memcpy(tempBuffer + bytesLoaded, rawData, newBytes);
		bytesLoaded += newBytes;

		rawData += newBytes;
		bytesAvailable -= newBytes;
	}

	if (bytesLoaded == bufferSize && rawSize == 0)
	{
		int16_t sugar;
		Serializer::read(sugar, tempBuffer);
		if (sugar != SUGAR)
		{
			std::string errorMessage = "Sugar mismatch. expected ";
			errorMessage += std::to_string((int)(SUGAR & 0xff));
			errorMessage += " ";
			errorMessage += std::to_string((int)((SUGAR >> 8) & 0xff));

			errorMessage += " received ";

			errorMessage += std::to_string((int)tempBuffer[0]);
			errorMessage += " ";
			errorMessage += std::to_string((int)tempBuffer[1]);
			S::log.add(errorMessage, { LOG_TAGS::NET, LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG });
			THROW_FATAL_ERROR("Invalid network message");
			return;
		}

		Serializer::read(payloadSize, tempBuffer + sizeof(int16_t));
		rawSize = payloadSize + bufferSize;

		this->rawData = new char[rawSize];
		std::memcpy(this->rawData, tempBuffer, bufferSize);

		payload = this->rawData + sizeof(int16_t) * 2;
	}

	if (bytesLoaded >= bufferSize)
	{		
		uint16_t bytesToRead = std::min(bytesAvailable, rawSize - bytesLoaded);
		std::memcpy(this->rawData + bytesLoaded, rawData, bytesToRead);
		bytesLoaded += bytesToRead;
	}
	
}

void NetworkPacket::tracePayload()
{
	S::log.add("NetworkPacket payload:\n\t" + Serializer::toHex(payload, payloadSize), { LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });
}

bool NetworkPacket::isFullyLoaded()
{
	return rawSize > 4 && bytesLoaded == rawSize;
}

void NetworkPacket::setPayloadFromRawData(const char* newRawData, size_t size)
{
	if (rawSize > 0)
		delete[] rawData;

	payloadSize = (uint16_t)size;
	rawSize = (uint16_t)size + sizeof(uint16_t) * 2;
	rawData = new char[rawSize];
	payload = rawData + sizeof(uint16_t) * 2;
	std::memcpy(payload, newRawData, size);
	Serializer::write((int16_t)SUGAR, rawData);
	Serializer::write((uint16_t)size, rawData + 2);

	bytesLoaded = rawSize;
}

template <>
void NetworkPacket::setPayloadFromSerializable<>(const NetworkMessage& serializable, size_t expectedSize)
{
	BinarySerializer bs;
	serializable.serialize(bs);
	auto data = bs.dumpAll();
	setPayloadFromRawData(data.data(), data.size());
}