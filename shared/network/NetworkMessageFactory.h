#pragma once
#include <NetworkMessage.h>

namespace NetworkMessageFactory
{
	std::unique_ptr<NetworkMessage> parse(const char* data, size_t size);	
	std::unique_ptr<NetworkMessage> parse(BinarySerializer& serializer);
};