#pragma once
#include <NetworkMessage.h>
#include <NetworkPacket.h>

class NetworkMessageFactory
{
public:
	std::unique_ptr<NetworkMessage> parse(const NetworkPacket& packet);
	std::unique_ptr<NetworkMessage> pointerByType(MESSAGE_TYPE type);	
};