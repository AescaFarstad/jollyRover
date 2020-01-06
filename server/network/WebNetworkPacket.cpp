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


void WebNetworkPacket::loadFromRawData(const char* incomingData, int bytesAvailable)
{
	//Load size of the packet size
	if (bytesLoaded < bufferSize1)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize1 - bytesLoaded);
		memcpy(tempBuffer1 + bytesLoaded, incomingData, newBytes);
		bytesLoaded += newBytes;

		incomingData += newBytes;
		bytesAvailable -= newBytes;
	}

	//Not enough info about size of the packet size -> return
	if (bytesLoaded < bufferSize1)
		return;

	//Packet size size (sic!) is loaded for the first time
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
			bufferSize2 = CYPHER_KEY_SIZE;
		}
		else if ((unsigned char)tempBuffer1[1] - 128 == 126)
		{
			bufferSize2 = 2 + CYPHER_KEY_SIZE; //actuall size as uint16_t + cypher key
		}
		else
		{
			THROW_FATAL_ERROR("int64-sized packets are not implemented yet");
		}
		tempBuffer2 = new char[bufferSize2];
	}

	//Loading packet size + keys
	if (bytesLoaded < bufferSize1 + bufferSize2)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize1 + bufferSize2 - bytesLoaded);
		memcpy(tempBuffer2 + bytesLoaded - bufferSize1, incomingData, newBytes);
		bytesLoaded += newBytes;

		incomingData += newBytes;
		bytesAvailable -= newBytes;
	}

	//When packet size + keys are fully loaded and can be parsed
	if (bytesLoaded == bufferSize1 + bufferSize2 && rawSize == 0)
	{
		int rawPayloadLength = 0;
		if (bufferSize2 == 4)
		{
			int8_t tmp;
			char* readable = static_cast<char*>(tempBuffer1) + 1;
			
			Serializer::read(tmp, readable);
			tmp -= 128;
			rawPayloadLength = tmp;
		}
		else
		{
			uint16_t tmp;
			char* readable = static_cast<char*>(tempBuffer2);
			
			Serializer::read(tmp, readable);
			if (!SystemInfo::isBigEndian)
				Serializer::swapBytes(&tmp);
			rawPayloadLength = tmp;
		}
		this->payload = new char[rawPayloadLength];
		this->payloadSize = rawPayloadLength;
		rawSize = bufferSize1 + bufferSize2 + this->payloadSize;
	}

	//Loading payload
	if (bytesLoaded >= bufferSize1 + bufferSize2)
	{
		uint16_t bytesToRead = std::min(bytesAvailable, rawSize - bytesLoaded);
		memcpy(this->payload + bytesLoaded - bufferSize1 - bufferSize2, incomingData, bytesToRead);
		bytesLoaded += bytesToRead;
	}


	//When all data is loaded -> decypher payload and clear temporaries
	if (rawSize > 0 && rawSize == bytesLoaded)
	{
		char* key = tempBuffer2 + bufferSize2 - CYPHER_KEY_SIZE;
		for (int i = 0; i < this->payloadSize; i++)
		{
			this->payload[i] = (this->payload[i] ^ key[i % CYPHER_KEY_SIZE]);
		}
		bufferSize2 = 0;
		delete[] tempBuffer2;
	}
}

void WebNetworkPacket::setPayloadFromRawData(const char* newRawData, size_t size)
{
	if (rawSize > 0)
		delete[] rawData;	

	payloadSize = (uint16_t)size;
	
	//payloadSize > 125 ? op code(1) + size of size(1) + size(2) : op code(1) + size(1)
	rawSize = payloadSize + (payloadSize > 125 ? 4 : 2);
	
	rawData = new char[rawSize];
	
	payload = rawData + rawSize - payloadSize;

	memcpy(payload, newRawData, size);
	
	int cursor = 0;
	rawData[cursor] = (unsigned char)130; //Binary, unmasked
	cursor++;

	if (payloadSize <= 125)
	{
		rawData[cursor] = (unsigned char)payloadSize;
		cursor++;
	}
	else
	{
		rawData[cursor] = (unsigned char)126;
		cursor++;
		
		uint16_t tmp = payloadSize;
			
		if (!SystemInfo::isBigEndian)
			Serializer::swapBytes(&tmp);
			
		Serializer::write(tmp, rawData + cursor);
		cursor += 2;
	}
}

