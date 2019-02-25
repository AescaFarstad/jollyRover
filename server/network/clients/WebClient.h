#pragma once
#include <NetworkClient.h>
#include <WebNetworkPacket.h>
#include <Global.h>
#include <SDL2/SDL_net.h>
#include <Serializer.h>
#include <functional>
#include <libwshandshake.hpp>
#include <regex>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <base64_2.h>

class WebClient :
	public NetworkClient
{
public:
	WebClient(
		std::function<void(NetworkClient* client)> onHttpHandshakeDone,
		std::function< int() > globalSocketNudgeFunction);
	virtual ~WebClient();

	bool isHandshakeDone;

	virtual std::unique_ptr<NetworkPacket> poll();
	virtual void sendMessage(NetworkMessage* msg);
	virtual void sendMessage(const char * payload, size_t size);
	virtual void init() override;

protected:

	static std::unique_ptr<NetworkPacket> getPacket();

private:
	std::function<void(NetworkClient* client)> onHttpHandshakeDone;
};

