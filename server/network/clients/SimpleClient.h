#pragma once
#include <NetworkClient.h>
#include <Global.h>
#include <SDL2/SDL_net.h>


class SimpleClient :
	public NetworkClient
{
public:
	SimpleClient(
		std::function<void(NetworkClient& client)> onHandshakeDone,
		std::function< int32_t() > globalSocketNudgeFunction);

	std::unique_ptr<NetworkPacket> poll() override;
	void sendMessage(const NetworkMessage& msg) override;
	void sendMessage(const char* payload, size_t size) override;

private:

	bool m_isHandshakeDone;
	std::function<void(NetworkClient& client)> m_onHandshakeDone;

};

