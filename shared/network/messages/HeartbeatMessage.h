#pragma once

#include <NetworkMessage.h>

class HeartbeatMessage :
	public NetworkMessage
{
public:
	HeartbeatMessage();

	std::string getName() const override;
};