#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <SDL_net.h>
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

class ServerNetwork
{
public:
	ServerNetwork();
	~ServerNetwork();

	NetworkMessageFactory factory;

	void init();
	void finish();
	void update(std::unique_ptr<NetworkMessage> externalBuffer[]);
	bool hasClients();
	int getClientCount();
	void sendToAllPlaying(NetworkMessage* message);
	void addPlayer(int32_t login);
	void send(NetworkMessage* message, int32_t login);
	void send(const char * payload, size_t size, int32_t login);


private:

	TCPsocket serverSocketRaw;
	TCPsocket serverSocketWeb;
	std::vector<NetworkClient*> clients;
	SDLNet_SocketSet socketSet;
	int clientCount;
	std::function<void(UndeterminedClient* client)> onClientDetermined;
	std::function<void(NetworkClient* client)> onHandshakeDone;
	//void onHandshakeDone(NetworkClient* client);
	SeededRandom random;

	void handleConnections();
	void handleData(std::unique_ptr<NetworkMessage> messageBuffer[]);
	void handlePacket(std::unique_ptr<NetworkPacket> packet, NetworkClient* client, std::unique_ptr<NetworkMessage> messageBuffer[]);
	void addMessageToBuffer(std::unique_ptr<NetworkMessage> msg, std::unique_ptr<NetworkMessage> messageBuffer[]);
	void generateNewLogin(int32_t &login);
	void generateNewPassword(std::vector<int8_t> &password);
	bool loginExists(int32_t login);
};

