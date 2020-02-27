#pragma once
#include <SDL2/SDL_net.h>
#include <NetworkMessage.h>
#include <NetworkClient.h>
#include <SimpleClient.h>
#include <NetworkPacket.h>
#include <UndeterminedClient.h>
#include <NetworkMessageFactory.h>
#include <ServerConstants.h>
#include <SeededRandom.h>
#include <VariableProto.h>

using MessageBuffer = std::array<std::unique_ptr<NetworkMessage>, SERVER_CONST::messageBufferSize + 1>;

class ServerNetwork
{
public:
	ServerNetwork();

	NetworkMessageFactory factory;

	void init(std::function<bool(int32_t)> isLoginAllowedToReconnect, std::function<bool(int32_t)> loginExists, VariableProto* vars);
	void update(MessageBuffer& externalBuffer);
	bool hasClients();
	int32_t getClientCount();
	void sendToAllPlaying(const NetworkMessage& message);
	void addPlayer(int32_t login);
	void send(NetworkMessage& message, int32_t login);
	void send(const char* payload, size_t size, int32_t login);


private:

	TCPsocket serverSocketRaw;
	TCPsocket serverSocketWeb;
	std::vector<std::unique_ptr<NetworkClient>> clients;
	SDLNet_SocketSet socketSet;
	int32_t clientCount;
	std::function<void(UndeterminedClient* client)> onClientDetermined;
	std::function<void(NetworkClient& client)> onHandshakeDone;
	std::function<bool(int32_t)> isLoginAllowedToReconnect;
	std::function<bool(int32_t)> loginExists;
	SeededRandom random;
	VariableProto* m_vars;

	void handleConnections();
	void handleData(MessageBuffer& externalBuffer);
	void handlePacket(std::unique_ptr<NetworkPacket> packet, NetworkClient& client, MessageBuffer& externalBuffer);
	void addMessageToBuffer(std::unique_ptr<NetworkMessage> msg, MessageBuffer& externalBuffer);
	void generateNewLogin(int32_t& login);
	void generateNewPassword(std::vector<int8_t>& password);
	bool isLoginAvailable(int32_t login);
};

