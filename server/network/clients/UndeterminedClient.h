#pragma once
#include <NetworkClient.h>
#include <Global.h>
#include <SDL2/SDL_net.h>

class UndeterminedClient :
	public NetworkClient
{
public:
	UndeterminedClient(
		std::function<void(UndeterminedClient* client)> onClientDetermined,
		std::function< int32_t() > globalSocketNudgeFunction);

	bool isSimpleClient;

	virtual std::unique_ptr<NetworkPacket> poll();
	void sendMessage(const NetworkMessage& msg) override;
	void sendMessage(const char* payload, size_t size) override;
private:
	std::function<void(UndeterminedClient* client)> onClientDetermined;
};

