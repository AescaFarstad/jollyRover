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

	bool isHandshakeDone;

	virtual std::unique_ptr<NetworkPacket> poll();
	virtual void sendMessage(const NetworkMessage& msg);
	virtual void sendMessage(const char* payload, size_t size);
	virtual void init() override;

protected:

	static std::unique_ptr<NetworkPacket> getPacket();

private:
	std::function<void(NetworkClient& client)> onHttpHandshakeDone;
};

