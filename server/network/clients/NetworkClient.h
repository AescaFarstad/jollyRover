#pragma once
#include <memory>
#include <SDL2/SDL_net.h>
#include <NetworkPacket.h>
#include <NetworkMessage.h>
#include <PacketReader.h>


enum class NETWORK_CLIENT_STATE : int8_t {
	UNDETERMINED,
	GREETING,
	CONNECTED,
	PLAYING
};

class NetworkClient
{
public:
	NetworkClient(std::function< int() > globalSocketNudgeFunction);
	virtual ~NetworkClient();

	TCPsocket socket;
	bool wasConnected;
	bool wasDisconnected;
	NETWORK_CLIENT_STATE state;
	int32_t login;
	std::vector<int8_t> password;

	virtual std::unique_ptr<NetworkPacket> poll();
	virtual void sendMessage(NetworkMessage* msg);
	virtual void sendMessage(const char * payload, size_t size);
	virtual void init();

protected:
	PacketReader* packetReader;
	bool isInitialized;

	void logSend(std::string messageName, NetworkPacket &packet);

	static std::unique_ptr<NetworkPacket> getPacket();
	std::function< int() > globalSocketNudgeFunction;
};
