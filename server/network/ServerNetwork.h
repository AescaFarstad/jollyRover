#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <SDL2/SDL_net.h>
#include <Global.h>
#include <ServerConstants.h>
#include <NetworkMessage.h>
#include <InputPlayerLeftMessage.h>
#include <NetworkClient.h>
#include <SimpleClient.h>
#include <WebClient.h>
#include <NetworkPacket.h>
#include <UndeterminedClient.h>
#include <NetworkMessageFactory.h>
#include <SeededRandom.h>

using MessageBuffer = std::array<std::unique_ptr<NetworkMessage>, SERVER_CONST::messageBufferSize + 1>;

class ServerNetwork
{
public:
	ServerNetwork();

	NetworkMessageFactory factory;

	void init();
	void update(MessageBuffer& externalBuffer);
	bool hasClients();
	int getClientCount();
	void sendToAllPlaying(const NetworkMessage& message);
	void addPlayer(int32_t login);
	void send(NetworkMessage& message, int32_t login);
	void send(const char* payload, size_t size, int32_t login);


private:

	TCPsocket serverSocketRaw;
	TCPsocket serverSocketWeb;
	std::vector<NetworkClient*> clients;
	SDLNet_SocketSet socketSet;
	int clientCount;
	std::function<void(UndeterminedClient* client)> onClientDetermined;
	std::function<void(NetworkClient& client)> onHandshakeDone;
	//void onHandshakeDone(NetworkClient* client);
	SeededRandom random;

	void handleConnections();
	void handleData(MessageBuffer& externalBuffer);
	void handlePacket(std::unique_ptr<NetworkPacket> packet, NetworkClient* client, MessageBuffer& externalBuffer);
	void addMessageToBuffer(std::unique_ptr<NetworkMessage> msg, MessageBuffer& externalBuffer);
	void generateNewLogin(int32_t &login);
	void generateNewPassword(std::vector<int8_t> &password);
	bool loginExists(int32_t login);
};

