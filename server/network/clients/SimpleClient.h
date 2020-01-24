#pragma once
#include <NetworkClient.h>
#include <Global.h>
#include <SDL2/SDL_net.h>


class SimpleClient :
	public NetworkClient
{
public:
	SimpleClient(std::function< int32_t() > globalSocketNudgeFunction);

	virtual void sendMessage(const NetworkMessage& msg);
	virtual void sendMessage(const char* payload, size_t size);

};

