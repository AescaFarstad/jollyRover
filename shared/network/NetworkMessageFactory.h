#pragma once
#include <NetworkPacket.h>

namespace NetworkMessageFactory
{
	std::unique_ptr<NetworkMessage> parse(const NetworkPacket& packet);
};