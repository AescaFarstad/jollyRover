#pragma once
#include <SDL2/SDL_net.h>
#include <NetworkPacket.h>
#include <NetworkMessage.h>
#include <PacketReader.h>
#include <ConnectionMonitor.h>


enum class NETWORK_CLIENT_STATE : int8_t {
	UNDETERMINED,
	GREETING,
	CONNECTED,
	PLAYING
};

class NetworkClient
{
public:
	NetworkClient(std::function< int32_t() > globalSocketNudgeFunction);
	virtual ~NetworkClient() = default;

	TCPsocket socket;
	bool wasConnected;
	bool wasDisconnected;
	NETWORK_CLIENT_STATE state;
	int32_t login;
	std::vector<int8_t> password;
	ConnectionMonitor monitor;
	std::vector<uint32_t> requestedStates;

	virtual std::unique_ptr<NetworkPacket> poll();
	virtual void sendMessage(const NetworkMessage& msg) = 0;
	virtual void sendMessage(const char* payload, size_t size) = 0;

protected:
	bool isInitialized;
	std::unique_ptr<PacketReader> packetReader;
	std::function< int32_t() > globalSocketNudgeFunction;

	void logSend(std::string messageName, NetworkPacket& packet);
	virtual void init();
	static std::unique_ptr<NetworkPacket> getPacket();
};
