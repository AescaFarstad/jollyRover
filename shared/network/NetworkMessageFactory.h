#pragma once
#include <NetworkPacket.h>

class NetworkMessageFactory
{
public:
	std::unique_ptr<NetworkMessage> parse(const NetworkPacket& packet);
};