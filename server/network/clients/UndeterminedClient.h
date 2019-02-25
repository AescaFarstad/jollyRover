#pragma once
#include <NetworkClient.h>
#include <Global.h>
#include <SDL2/SDL_net.h>
#include <Serializer.h>
#include <functional>

class UndeterminedClient :
	public NetworkClient
{
public:
	UndeterminedClient(
		std::function<void(UndeterminedClient* client)> onClientDetermined,
		std::function< int() > globalSocketNudgeFunction);
	virtual ~UndeterminedClient();

	bool isSimpleClient;

	virtual std::unique_ptr<NetworkPacket> poll();
	void clear();
private:
	std::function<void(UndeterminedClient* client)> onClientDetermined;
};

