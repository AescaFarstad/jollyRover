#pragma once
#include <NetworkClient.h>
#include <SDL2/SDL_net.h>
#include <functional>

class WebClient :
	public NetworkClient
{
public:
	WebClient(
		std::function<void(NetworkClient& client)> onHttpHandshakeDone,
		std::function< int32_t() > globalSocketNudgeFunction);
	virtual ~WebClient() = default;

	std::unique_ptr<NetworkPacket> poll() override;
	void sendMessage(const NetworkMessage& msg) override;
	void sendMessage(const char* payload, size_t size) override;
	void init() override;

protected:

	static std::unique_ptr<NetworkPacket> getPacket();

private:

	bool m_isHandshakeDone;
	std::function<void(NetworkClient& client)> m_onHttpHandshakeDone;
};

