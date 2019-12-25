#include <ServerNetwork.h>



ServerNetwork::ServerNetwork()
{
	clientCount = 0;

	onClientDetermined = [this, &clients = clients, &onHandshakeDone = onHandshakeDone](UndeterminedClient* client) {
		NetworkClient* newClient = nullptr;
		if (client->isSimpleClient)
		{
			SimpleClient* newSimpleClient = new SimpleClient([this]() {
				return SDLNet_CheckSockets(socketSet, 0);
			});
			auto index = std::find(clients.begin(), clients.end(), client);
			*index = newSimpleClient;
			newSimpleClient->socket = client->socket;
			newClient = newSimpleClient;
		}
		delete client;
		if (newClient != nullptr)
		{
			//onHandshakeDone(newClient);
			newClient->state = NETWORK_CLIENT_STATE::GREETING;
			GenericRequestMessage grm;
			grm.request = REQUEST_TYPE::REQUEST_GREETING;
			newClient->sendMessage(&grm);
		}
	};

	onHandshakeDone = [](NetworkClient* client) {
		client->state = NETWORK_CLIENT_STATE::GREETING;
		GenericRequestMessage grm;
		grm.request = REQUEST_TYPE::REQUEST_GREETING;
		client->sendMessage(&grm);
	};
}


ServerNetwork::~ServerNetwork()
{
}

void ServerNetwork::init()
{
	IPaddress ip;
	SDLNet_ResolveHost(&ip, NULL, S::config.port);
	serverSocketRaw = SDLNet_TCP_Open(&ip);

	SDLNet_ResolveHost(&ip, NULL, S::config.webPort);
	serverSocketWeb = SDLNet_TCP_Open(&ip);

	std::vector<TCPsocket> clients;
	socketSet = SDLNet_AllocSocketSet(SERVER_CONST::maxNumberOfConnections);
}

void ServerNetwork::finish()
{
}

void ServerNetwork::update(std::unique_ptr<NetworkMessage> externalBuffer[])
{
	handleConnections();
	handleData(externalBuffer);
}

bool ServerNetwork::hasClients()
{
	return clientCount > 0;
}

int ServerNetwork::getClientCount()
{
	return clientCount;
}

void ServerNetwork::sendToAllPlaying(NetworkMessage * message)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == nullptr)
			continue;
		if (clients[i]->state == NETWORK_CLIENT_STATE::PLAYING)
			clients[i]->sendMessage(message);
	}
}

void ServerNetwork::addPlayer(int32_t login)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == nullptr)
			continue;
		if (clients[i]->login == login)
		{
			clients[i]->state = NETWORK_CLIENT_STATE::PLAYING;
			break;
		}

	}
}

void ServerNetwork::send(NetworkMessage * message, int32_t login)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == nullptr)
			continue;
		if (clients[i]->login == login)
		{
			message->stamp = SDL_GetTicks();
			message->initiator_id = NetworkMessage::getMessageId();
			clients[i]->sendMessage(message);
			return;
		}
	}
}

void ServerNetwork::send(const char * payload, size_t size, int32_t login)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i] == nullptr)
			continue;
		if (clients[i]->login == login)
		{
			clients[i]->sendMessage(payload, size);
			return;
		}
	}
}

void ServerNetwork::handleConnections()
{
	TCPsocket clientSocket = SDLNet_TCP_Accept(serverSocketRaw);
	if (clientSocket)
	{
		S::log.add("Client connected R", {LOG_TAGS::NET, LOG_TAGS::NET_BRIEF});
		UndeterminedClient* uclient = new UndeterminedClient(onClientDetermined, [this]() {
			return SDLNet_CheckSockets(socketSet, 0);
		});
		uclient->socket = clientSocket;
		clients.push_back(uclient);
		SDLNet_TCP_AddSocket(socketSet, clientSocket);
		clientCount++;
	}

	clientSocket = SDLNet_TCP_Accept(serverSocketWeb);
	if (clientSocket)
	{
		S::log.add("Client connected W", { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF });
		WebClient* wclient = new WebClient(onHandshakeDone, [this]() {
			return SDLNet_CheckSockets(socketSet, 0);
		});
		wclient->socket = clientSocket;
		clients.push_back(wclient);
		SDLNet_TCP_AddSocket(socketSet, clientSocket);
		clientCount++;
	}
}

void ServerNetwork::handleData(std::unique_ptr<NetworkMessage> externalBuffer[])
{
	int active = SDLNet_CheckSockets(socketSet, 1);
	if (active <= 0)
		return;

	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i] == nullptr)
			continue;
		while (true)
		{
			std::unique_ptr<NetworkPacket> packet = clients[i]->poll();
			if (packet)
			{
				S::log.add("> incoming " + std::to_string(packet->payloadSize) + ":\n\t" + Serializer::toHex(packet->payload, packet->payloadSize), { LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });
				handlePacket(std::move(packet), clients[i], externalBuffer);
			}
			else
			{
				break;
			}
		}
		if (clients[i]->wasDisconnected)
		{
			SDLNet_TCP_DelSocket(socketSet, clients[i]->socket);
			SDLNet_TCP_Close(clients[i]->socket);

			if (clients[i]->state == NETWORK_CLIENT_STATE::PLAYING)
			{
				std::unique_ptr<InputPlayerLeftMessage> disconnectedMessage = std::make_unique<InputPlayerLeftMessage>();
				disconnectedMessage->login = clients[i]->login;
				addMessageToBuffer(std::move(disconnectedMessage), externalBuffer);
			}

			S::log.add("Client disconnected " + std::to_string(clients[i]->login), { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF });

			delete clients[i];
			clients[i] = nullptr;
			clientCount--;
		}
	}
}



void ServerNetwork::handlePacket(std::unique_ptr<NetworkPacket> packet, NetworkClient* client, std::unique_ptr<NetworkMessage> externalBuffer[])
{
	std::unique_ptr<NetworkMessage> msg = factory.parse(packet.get());
	msg->login = client->login;
	switch (msg->typeId)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG:
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG:
		case MESSAGE_TYPE::TYPE_REQUEST_MSG:
		{
			addMessageToBuffer(std::move(msg), externalBuffer);
			break;
		}
		case MESSAGE_TYPE::TYPE_GREETING_MSG:
		{
			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(msg.get());
			//TODO check login, password
			generateNewLogin(gMsg->login);
			generateNewPassword(gMsg->password);
			client->login = gMsg->login;
			client->password = gMsg->password;
			gMsg->inResponseTo = gMsg->initiator_id;
			gMsg->stamp = SDL_GetTicks();
			client->sendMessage(gMsg);
			client->wasConnected = true;
			client->state = NETWORK_CLIENT_STATE::CONNECTED;
			break;
		}
		default:
		{	
			S::log.add("Message type not handled: " + std::to_string((int16_t)msg->typeId), { LOG_TAGS::ERROR_ });
			break;
		}
	}
}

void ServerNetwork::addMessageToBuffer(std::unique_ptr<NetworkMessage> msg, std::unique_ptr<NetworkMessage> externalBuffer[])
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

void ServerNetwork::generateNewLogin(int32_t &login)
{
	do
	{
		login = random.get(100, 1000);
	} 
	while (loginExists(login));
}

void ServerNetwork::generateNewPassword(std::vector<int8_t> &password)
{
	password.clear();
	for (size_t i = 0; i < 12; i++)
	{
		password.push_back((int8_t)random.get(1, 0xff));
	}
}

bool ServerNetwork::loginExists(int32_t login)
{
	for (size_t i = 0; i < clients.size(); i++)
	{

		if (clients[i] == nullptr)
			continue;
		if (clients[i]->login == login)
			return true;
	}
	return false;
}



