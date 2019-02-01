#include "WebNetworkPacket.h"

//const char* WebNetworkPacket::DEEB_DAAB = "\xde\xed\xda\xab";

WebNetworkPacket::WebNetworkPacket()
{
	bufferSize2 = 0;
	rawSize = 0;
}


WebNetworkPacket::~WebNetworkPacket()
{
	if (rawSize > 0 && rawData == nullptr)
		delete[] payload;
	if (bufferSize2 != 0)
		delete[] tempBuffer2;
}


void WebNetworkPacket::loadFromRawData(const char* rawData, int bytesAvailable)
{
	//Load size of packet size
	if (bytesLoaded < bufferSize1)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize1 - bytesLoaded);
		memcpy(tempBuffer1 + bytesLoaded, rawData, newBytes);
		bytesLoaded += newBytes;

		rawData += newBytes;
		bytesAvailable -= newBytes;
	}

	//No info about size of packet size -> return
	if (bytesLoaded < bufferSize1)
		return;

	//Packet size size (sic!) has just been received. Parse it.
	if (bytesLoaded == bufferSize1 && bufferSize2 == 0)
	{
		unsigned char opcode = (unsigned char)tempBuffer1[0];
		if (opcode != 130)
		{
			if (opcode == 136)
			{
				isDisconnectNotice = true;
				return;
			}
			else
			{
				printf("opcode %d\n", opcode);
				THROW_FATAL_ERROR("WS OPCODE mismatch");
			}
		}

		if ((unsigned char)tempBuffer1[1] - 128 < 126)
		{
			bufferSize2 = 4;
		}
		else if ((unsigned char)tempBuffer1[1] - 128 == 126)
		{
			bufferSize2 = 6;
		}
		else
		{
			THROW_FATAL_ERROR("int64-sized packets are not implemented yet");
		}
		tempBuffer2 = new char[bufferSize2];
	}

	//Wait untill final packet size info
	if (bytesLoaded < bufferSize1 + bufferSize2)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize1 + bufferSize2 - bytesLoaded);
		memcpy(tempBuffer2 + bytesLoaded - bufferSize1, rawData, newBytes);
		bytesLoaded += newBytes;

		rawData += newBytes;
		bytesAvailable -= newBytes;
	}

	//Final Packet size has just been received
	if (bytesLoaded == bufferSize1 + bufferSize2 && rawSize == 0)
	{
		int rawPayloadLength = 0;
		if (bufferSize2 == 4)
		{
			int8_t tmp;
			char* readable = const_cast<char*>(tempBuffer1) + 1;
			Serializer::read(tmp, readable);
			tmp -= 128;
			rawPayloadLength = tmp;
		}
		else
		{
			int16_t tmp;
			char* readable = const_cast<char*>(tempBuffer2);
			Serializer::read(tmp, readable);
			rawPayloadLength = tmp;
		}
		this->payload = new char[rawPayloadLength];
		this->payloadSize = rawPayloadLength;
		rawSize = bufferSize1 + bufferSize2 + this->payloadSize;
	}

	//accumulate payload
	if (bytesLoaded >= bufferSize1 + bufferSize2)
	{
		uint16_t bytesToRead = std::min(bytesAvailable, rawSize - bytesLoaded);
		memcpy(this->payload, rawData, bytesToRead);
		bytesLoaded += bytesToRead;
	}


	//decypher payload
	if (rawSize > 0 && rawSize == bytesLoaded)
	{
		char* key = tempBuffer2 + bufferSize2 - 4;
		for (int i = 0; i < this->payloadSize; i++)
		{
			this->payload[i] = (this->payload[i] ^ key[i % 4]);
		}
		bufferSize2 = 0;
		delete[] tempBuffer2;
	}
}

void WebNetworkPacket::setPayloadFromRawData(const char * newRawData, size_t size)
{
	if (rawSize > 0)
		delete[] rawData;	

	payloadSize = (int16_t)size;
	rawSize = payloadSize + (payloadSize > 125 ? 4 : 2);
	rawData = new char[rawSize];
	payload = rawData + rawSize - payloadSize;

	memcpy(payload, newRawData, size);

	int cursor = 0;
	rawData[cursor] = (unsigned char)130;
	cursor++;

	if (payloadSize <= 125)
	{
		rawData[cursor] = (unsigned char)payloadSize;
		cursor++;
	}
	else
	{
		Serializer::write(payloadSize, rawData + cursor + 1);
		cursor += 3;
	}
}

