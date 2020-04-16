#pragma once
#include <SDL2/SDL_net.h>
#include <NetworkMessage.h>
#include <NetworkClient.h>
#include <SimpleClient.h>
#include <NetworkPacket.h>
#include <NetworkMessageFactory.h>
#include <ServerConstants.h>
#include <SeededRandom.h>
#include <VariableProto.h>

using MessageBuffer = std::array<std::unique_ptr<NetworkMessage>, SERVER_CONST::messageBufferSize + 1>;

class ServerNetwork
{
public:
	ServerNetwork();

	void init(std::function<bool(int32_t)> isLoginAllowedToReconnect, std::function<bool(int32_t)> loginExists, VariableProto* vars);
	void update(MessageBuffer& externalBuffer);
	bool hasClients();
	int32_t getClientCount();
	NetworkClient* clientByLogin(int32_t login);
	void sendToAllPlaying(const NetworkMessage& message);
	void addPlayer(int32_t login);
	void send(NetworkMessage& message, int32_t login);
	void send(const char* payload, size_t size, int32_t login);


private:

	TCPsocket m_serverSocketRaw;
	TCPsocket m_serverSocketWeb;
	std::vector<std::unique_ptr<NetworkClient>> m_clients;
	SDLNet_SocketSet m_socketSet;
	int32_t m_clientCount;
	std::function<void(NetworkClient& client)> m_onHandshakeDone;
	std::function<bool(int32_t)> m_isLoginAllowedToReconnect;
	std::function<bool(int32_t)> m_loginExists;
	SeededRandom m_random;
	VariableProto* m_vars;

	void handleConnections();
	void addClient(std::unique_ptr<NetworkClient> client, TCPsocket& clientSocket);
	void handleData(MessageBuffer& externalBuffer);
	void handlePacket(std::unique_ptr<NetworkPacket> packet, NetworkClient& client, MessageBuffer& externalBuffer);
	void addMessageToBuffer(std::unique_ptr<NetworkMessage> msg, MessageBuffer& externalBuffer);
	void generateNewLogin(int32_t& login);
	void generateNewPassword(std::vector<int8_t>& password);
	bool isLoginAvailable(int32_t login);
};

