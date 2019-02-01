#pragma once
#include <NetworkClient.h>
#include <Global.h>
#include <SDL_net.h>
#include <NetworkPacket.h>
#include <NetworkMessage.h>
#include <ReadOnlySerializationStream.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <string>


class SimpleClient :
	public NetworkClient
{
public:
	SimpleClient(std::function< int() > globalSocketNudgeFunction);
	~SimpleClient();

	virtual void sendMessage(NetworkMessage* msg);
	virtual void sendMessage(const char * payload, size_t size);

};

