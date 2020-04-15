#include <ServerNetwork.h>
#include <SerializeSimpleTypes.h>
#include <GenericRequestMessage.h>
#include <GreetingMessage.h>
#include <WebClient.h>
#include <InputPlayerLeftMessage.h>
#include <Global.h>
#include <HeartbeatMessage.h>


ServerNetwork::ServerNetwork()
{
	m_clientCount = 0;
}

void ServerNetwork::init(std::function<bool(int32_t)> m_isLoginAllowedToReconnect, std::function<bool(int32_t)> m_loginExists, VariableProto* vars)
{
	m_vars = vars;
	
	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, S::config.port);
	m_serverSocketRaw = SDLNet_TCP_Open(&ip);

	SDLNet_ResolveHost(&ip, NULL, S::config.webPort);
	m_serverSocketWeb = SDLNet_TCP_Open(&ip);

	m_socketSet = SDLNet_AllocSocketSet(SERVER_CONST::maxNumberOfConnections);
	
	this->m_isLoginAllowedToReconnect = m_isLoginAllowedToReconnect;
	this->m_loginExists = m_loginExists;

	m_onHandshakeDone = [this](NetworkClient& client) {
		
		client.monitor.begin(SDL_GetTicks(), m_vars->heartbeatInterval, m_vars->heartbeatTimeout); //TODO handle timeout before first message
		client.state = NETWORK_CLIENT_STATE::GREETING;
		GenericRequestMessage grm;
		grm.request = REQUEST_TYPE::REQUEST_GREETING;
		client.sendMessage(grm);
		
	};
}

void ServerNetwork::update(MessageBuffer& externalBuffer)
{
	handleConnections();
	handleData(externalBuffer);
}

bool ServerNetwork::hasClients()
{
	return m_clientCount > 0;
}

int32_t ServerNetwork::getClientCount()
{
	return m_clientCount;
}

NetworkClient* ServerNetwork::clientByLogin(int32_t login)
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (m_clients[i]->login == login)
			return m_clients[i].get();
	}
	return nullptr;
}

void ServerNetwork::sendToAllPlaying(const NetworkMessage& message)
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (m_clients[i]->state == NETWORK_CLIENT_STATE::PLAYING)
			m_clients[i]->sendMessage(message);
	}
}

void ServerNetwork::addPlayer(int32_t login)
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (m_clients[i]->login == login)
		{
			m_clients[i]->state = NETWORK_CLIENT_STATE::PLAYING;
			break;
		}
	}
}

void ServerNetwork::send(NetworkMessage& message, int32_t login)
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (m_clients[i]->login == login)
		{
			message.stamp = SDL_GetTicks();
			m_clients[i]->sendMessage(message);
			return;
		}
	}
}

void ServerNetwork::send(const char* payload, size_t size, int32_t login)
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (m_clients[i]->login == login)
		{
			m_clients[i]->sendMessage(payload, size);
			return;
		}
	}
}

void ServerNetwork::handleConnections()
{
	TCPsocket clientSocket = SDLNet_TCP_Accept(m_serverSocketRaw);
	if (clientSocket)
	{
		S::log.add("Client connected R", {LOG_TAGS::NET, LOG_TAGS::NET_BRIEF, LOG_TAGS::HARD_LOG});
		addClient(std::make_unique<SimpleClient>(m_onHandshakeDone, [this]() {
				return SDLNet_CheckSockets(m_socketSet, 0);
			}),
			clientSocket
		);
	}

	clientSocket = SDLNet_TCP_Accept(m_serverSocketWeb);
	if (clientSocket)
	{
		S::log.add("Client connected W", { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF, LOG_TAGS::HARD_LOG });
		addClient(std::make_unique<WebClient>(m_onHandshakeDone, [this]() {
				return SDLNet_CheckSockets(m_socketSet, 0);
			}),
			clientSocket
		);
	}
}

void ServerNetwork::addClient(std::unique_ptr<NetworkClient> client, TCPsocket& clientSocket)
{
	client->socket = clientSocket;
	client->connectionStamp = SDL_GetTicks();
	m_clients.push_back(std::move(client));
	SDLNet_TCP_AddSocket(m_socketSet, clientSocket);
	m_clientCount++;
}

void ServerNetwork::handleData(MessageBuffer& externalBuffer)
{
	int32_t active = SDLNet_CheckSockets(m_socketSet, 1);
		
	auto time = SDL_GetTicks();

	for (size_t i = 0; i < m_clients.size(); ++i)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (active > 0)
		{
			while (true)
			{
				std::unique_ptr<NetworkPacket> packet = m_clients[i]->poll();
				if (packet)
				{
					S::log.add("> incoming " + std::to_string(packet->payloadSize) + ":\n\t" + Serializer::toHex(packet->payload, packet->payloadSize), { LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });
					handlePacket(std::move(packet), *m_clients[i], externalBuffer);
				}
				else
				{
					break;
				}
			}
		}
		if (m_clients[i]->wasDisconnected || m_clients[i]->monitor.isDisconnected(time))
		{
			SDLNet_TCP_DelSocket(m_socketSet, m_clients[i]->socket);
			SDLNet_TCP_Close(m_clients[i]->socket);

			if (m_clients[i]->state == NETWORK_CLIENT_STATE::PLAYING)
			{
				std::unique_ptr<InputPlayerLeftMessage> disconnectedMessage = std::make_unique<InputPlayerLeftMessage>();
				disconnectedMessage->playerLogin = m_clients[i]->login;
				disconnectedMessage->login = m_clients[i]->login;
				addMessageToBuffer(std::move(disconnectedMessage), externalBuffer);
			}
			
			std::string reason = m_clients[i]->wasDisconnected ? " (DC)" : " (missing heartbeat)";
			
			S::log.add("Client disconnected " + std::to_string(m_clients[i]->login) + reason + 
					"\nIt has been active for " + std::to_string((SDL_GetTicks() - m_clients[i]->connectionStamp)/1000) + " sec", { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF, LOG_TAGS::HARD_LOG });

			m_clients[i] = nullptr;
			m_clientCount--;
		}
		else if (m_clients[i]->monitor.pollHeartbeat(time))
		{
			m_clients[i]->sendMessage(HeartbeatMessage());
		}
	}
}

void ServerNetwork::handlePacket(std::unique_ptr<NetworkPacket> packet, NetworkClient& client, MessageBuffer& externalBuffer)
{
	std::unique_ptr<NetworkMessage> msg = factory.parse(*packet);
	S::log.add("> RCVD " + msg->getName() + "[" + std::to_string(packet->payloadSize) + "] from " + std::to_string(client.login), {LOG_TAGS::NET_BRIEF});
	msg->login = client.login;
	switch (msg->typeId)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG:
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG:
		case MESSAGE_TYPE::TYPE_REQUEST_MSG:
		case MESSAGE_TYPE::TYPE_CHECKSUM_MSG:
		case MESSAGE_TYPE::TYPE_GAME_STATE_MSG:
		{
			addMessageToBuffer(std::move(msg), externalBuffer);
			break;
		}
		case MESSAGE_TYPE::TYPE_GREETING_MSG:
		{
			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(msg.get());
			//TODO check login, password
			if (!m_isLoginAllowedToReconnect(gMsg->login))			
				generateNewLogin(gMsg->login);
			generateNewPassword(gMsg->password);
			client.login = gMsg->login;
			client.password = gMsg->password;
			gMsg->inResponseTo = gMsg->initiator_id;
			gMsg->stamp = SDL_GetTicks();
			client.sendMessage(*gMsg);
			client.wasConnected = true;
			client.state = NETWORK_CLIENT_STATE::CONNECTED;
			break;
		}
		case MESSAGE_TYPE::TYPE_HEARTBEAT_MSG:
		{
			client.monitor.onHearbeatMessage(SDL_GetTicks());
			break;
		}
		default:
		{	
			S::log.add("Message type not handled: " + std::to_string((int16_t)msg->typeId), { LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG });
			break;
		}
	}
}

void ServerNetwork::addMessageToBuffer(std::unique_ptr<NetworkMessage> msg, MessageBuffer& externalBuffer)
{
	for (size_t i = 0; true; i++)
	{
		if (externalBuffer[i] == nullptr)
		{
			externalBuffer[i] = std::move(msg);
			return;
		}
	}
}

void ServerNetwork::generateNewLogin(int32_t& login)
{
	do
	{
		login = m_random.get(100, 1000);
	} 
	while (!isLoginAvailable(login));
}

void ServerNetwork::generateNewPassword(std::vector<int8_t>& password)
{
	password.clear();
	for (size_t i = 0; i < 12; i++)
	{
		password.push_back((int8_t)m_random.get(1, 0xff));
	}
}

bool ServerNetwork::isLoginAvailable(int32_t login)
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == nullptr)
			continue;
		if (m_clients[i]->login == login)
			return false;
	}
	return !m_loginExists(login);
}



