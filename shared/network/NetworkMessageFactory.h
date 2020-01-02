#pragma once
#include <memory>
#include <NetworkMessage.h>
#include <NetworkPacket.h>


class NetworkMessageFactory
{
public:

	std::unique_ptr<NetworkMessage> parse(NetworkPacket* packet);

	std::unique_ptr<NetworkMessage> pointerByType(MESSAGE_TYPE type);
	
};