#include <WebNetworkPacket.h>
#include <SerializeSimpleTypes.h>
#include <Global.h>
#include <SystemInfo.h>

//const char* WebNetworkPacket::DEEB_DAAB = "\xde\xed\xda\xab";

WebNetworkPacket::WebNetworkPacket()
{
	bufferSize_p2 = 0;
	rawSize = 0;
}


WebNetworkPacket::~WebNetworkPacket()
{
	if (rawSize > 0 && rawData == nullptr)
		delete[] payload;
	if (bufferSize_p2 != 0)
		delete[] headerBuffer_p2;
}


void WebNetworkPacket::loadFromRawData(const char* incomingData, int32_t bytesAvailable)
{
	//Load size of the packet size
	if (bytesLoaded < bufferSize_p1)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize_p1 - bytesLoaded);
		std::memcpy(headerBuffer_p1 + bytesLoaded, incomingData, newBytes);
		bytesLoaded += newBytes;

		incomingData += newBytes;
		bytesAvailable -= newBytes;
	}

	//Not enough info about size of the packet size -> return
	if (bytesLoaded < bufferSize_p1)
		return;

	//Packet size size (sic!) is loaded for the first time
	if (bytesLoaded == bufferSize_p1 && bufferSize_p2 == 0)
	{
		unsigned char opcode = (unsigned char)headerBuffer_p1[0];
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

		if ((unsigned char)headerBuffer_p1[1] - 128 < 126)
		{
			bufferSize_p2 = CYPHER_KEY_SIZE;
		}
		else if ((unsigned char)headerBuffer_p1[1] - 128 == 126)
		{
			bufferSize_p2 = 2 + CYPHER_KEY_SIZE; //actuall size as uint16_t + cypher key
		}
		else
		{
			THROW_FATAL_ERROR("int64-sized packets are not implemented yet");
		}
		headerBuffer_p2 = new char[bufferSize_p2];
	}

	//Loading packet size + keys
	if (bytesLoaded < bufferSize_p1 + bufferSize_p2)
	{
		auto newBytes = std::min(bytesAvailable, bufferSize_p1 + bufferSize_p2 - bytesLoaded);
		std::memcpy(headerBuffer_p2 + bytesLoaded - bufferSize_p1, incomingData, newBytes);
		bytesLoaded += newBytes;

		incomingData += newBytes;
		bytesAvailable -= newBytes;
	}

	//When packet size + keys are fully loaded and can be parsed
	if (bytesLoaded == bufferSize_p1 + bufferSize_p2 && rawSize == 0)
	{
		int32_t rawPayloadLength = 0;
		if (bufferSize_p2 == 4)
		{
			int8_t tmp;
			char* readable = static_cast<char*>(headerBuffer_p1) + 1;
			
			Serializer::read(tmp, readable);
			tmp -= 128;
			rawPayloadLength = tmp;
		}
		else
		{
			uint16_t tmp;
			char* readable = static_cast<char*>(headerBuffer_p2);
			
			Serializer::read(tmp, readable);
			if (!SystemInfo::isBigEndian)
				Serializer::swapBytes(&tmp);
			rawPayloadLength = tmp;
		}
		this->payload = new char[rawPayloadLength];
		this->payloadSize = rawPayloadLength;
		rawSize = bufferSize_p1 + bufferSize_p2 + this->payloadSize;
	}

	//Loading payload
	if (bytesLoaded >= bufferSize_p1 + bufferSize_p2)
	{
		uint16_t bytesToRead = std::min(bytesAvailable, rawSize - bytesLoaded);
		std::memcpy(this->payload + bytesLoaded - bufferSize_p1 - bufferSize_p2, incomingData, bytesToRead);
		bytesLoaded += bytesToRead;
	}


	//When all data is loaded -> decypher payload and clear temporaries
	if (rawSize > 0 && rawSize == bytesLoaded)
	{
		char* key = headerBuffer_p2 + bufferSize_p2 - CYPHER_KEY_SIZE;
		for (int32_t i = 0; i < this->payloadSize; i++)
		{
			this->payload[i] = (this->payload[i] ^ key[i % CYPHER_KEY_SIZE]);
		}
		bufferSize_p2 = 0;
		delete[] headerBuffer_p2;
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

	std::memcpy(payload, newRawData, size);
	
	int32_t cursor = 0;
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

