#include <Network.h>

Network::Network()
{
	packetReader = new PacketReader(&socket, getNewPacket, [this]() {
		activeSockets = SDLNet_CheckSockets(socketSet, 0);
		if (activeSockets == -1)
		{
			S::log.add("SDLNet_CheckSockets: " + std::string(SDLNet_GetError()), { LOG_TAGS::NET, LOG_TAGS::ERROR_ });
			isConnected = false;
		}
		if (activeSockets <= 0)
			return 0;
		return SDLNet_SocketReady(socket); 
	});

	//----------------------------------

	auto handleGenericRequest = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message) {
		if (!genericRequestBinder.process(std::move(message)))
		{
			GenericRequestMessage* t = dynamic_cast<GenericRequestMessage*>(message.get());
			S::log.add("GenericRequestMessage is not handled: " + t->getName() + " " + std::to_string(t->request), { LOG_TAGS::NET });
		}
	});
	
	std::unique_ptr<AnonymousBinding> binding = std::make_unique<AnonymousBinding>("Network generic binding");
	binding->
	bindByType(MessageTypes::TYPE_REQUEST_MSG)->
	setCallOnce(false)->
	setHandler(std::move(handleGenericRequest));
	binder.bind(std::move(binding));

	//----------------------------------
}

Network::~Network()
{
	delete packetReader;
}

void Network::connect()
{
	SDLNet_Init();
	IPaddress ip;
	SDLNet_ResolveHost(&ip, GAME_CONFIG::host, GAME_CONFIG::port);
	socket = SDLNet_TCP_Open(&ip);

	S::log.add("connect status: " + std::string(socket == nullptr ? "false" : "true"), {LOG_TAGS::NET});
	S::log.add("ip, port: " + std::string(GAME_CONFIG::host) + " " + std::to_string(GAME_CONFIG::port), {LOG_TAGS::NET});

	SDLNet_TCP_AddSocket(socketSet, socket);
	isConnected = socket != nullptr;
	if (isConnected && !GAME_CONFIG::IS_WEB)
	{
		char codeMsg[4];
		Serializer::write(GAME_CONFIG::simpleClientCode, codeMsg);
		SDLNet_TCP_Send(socket, codeMsg, 4);
	}
}

void Network::update()
{
	if (!isConnected)
		return;

	activeSockets = SDLNet_CheckSockets(socketSet, GAME_CONFIG::networkUpdateInterval);

	if (activeSockets == -1)
	{
		S::log.add("SDLNet_CheckSockets: " + std::string(SDLNet_GetError()), { LOG_TAGS::NET, LOG_TAGS::ERROR_ });
		isConnected = false;
		return;
	}

	std::unique_ptr<NetworkMessage> message = poll();
	while (message != nullptr)
	{
		NetworkMessage* msgP = message.get();
		if (!binder.process(std::move(message)))
		{
			S::log.add("NetworkMessage is not handled: " + msgP->getName(), { LOG_TAGS::NET });
		}
		message = poll();
	}
}

void Network::send(NetworkPacket* packet)
{
	if (socket == nullptr)
	{
		S::log.add("Message will not be sent, not connected.", { LOG_TAGS::NET, LOG_TAGS::ERROR_ });
	}
	else
	{
		SDLNet_TCP_Send(socket, packet->rawData, packet->rawSize);
	}
}

void Network::send(NetworkMessage* msg)
{
	msg->initiator_id = NetworkMessage::getMessageId();

	std::unique_ptr<NetworkPacket> packet = getNewPacket();
	packet->setPayloadFromSerializable(msg);

	S::log.add("SEND " + msg->getName() + " " + std::to_string(packet->payloadSize) + ":\n\t" +
		Serializer::toHex(packet->payload, packet->payloadSize),
		{ LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });

	requestTimeByInitiatorId[msg->initiator_id] = SDL_GetTicks();
	send(packet.get());
}

std::unique_ptr<NetworkMessage> Network::poll()
{
	if (!isConnected || activeSockets <= 0)
		return nullptr;

	packetReader->setDataAvailable(SDLNet_SocketReady(socket));
	std::unique_ptr<NetworkPacket> temp = packetReader->poll();
	if (!temp)
	{
		return nullptr;
	}
	else if (temp->isDisconnectNotice)
	{
		isConnected = false;
		S::log.add("disconnected",	{ LOG_TAGS::NET });
		//TODO initiate reconnect
		return nullptr;
	}


	S::log.add("incoming (" + std::to_string(temp->payloadSize) + "):\n\t" +
		Serializer::toHex(temp->payload, temp->payloadSize),
		{ LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });

	std::unique_ptr<NetworkMessage> resultMessage = factory.parse(temp.get());

	//auto t = requestTimeByInitiatorId[resultMessage->inResponseTo];
	int32_t ticks = SDL_GetTicks();
	if (resultMessage->inResponseTo > 0 &&
		requestTimeByInitiatorId[resultMessage->inResponseTo] &&
		resultMessage->stamp != -1) //resultMessage->stamp is unsigned! 0_o
	{
		int32_t uncertaintyBefore = timeSync.getUncertainty();
		timeSync.addMeasurement(requestTimeByInitiatorId[resultMessage->inResponseTo], resultMessage->stamp, ticks);
		int32_t uncertaintyAfter = timeSync.getUncertainty();

		if (uncertaintyAfter < uncertaintyBefore || true)
			S::log.add("TimeSync range: " + std::to_string(uncertaintyBefore) + " -> " + std::to_string(uncertaintyAfter), { LOG_TAGS::NET });
	}

	S::log.add("RCVD " + resultMessage->getName(), { LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });

	if (resultMessage->typeId == MessageTypes::TYPE_REQUEST_MSG)
	{
		GenericRequestMessage* grM = static_cast<GenericRequestMessage*>(resultMessage.get());
		auto handlerIter = interceptorsGeneric_once.find(grM->request);
		if (handlerIter != interceptorsGeneric_once.end())
		{
			resultMessage.release();
			std::unique_ptr<GenericRequestMessage> resultGRMessage = std::unique_ptr<GenericRequestMessage>(grM);

			auto handler = handlerIter->second;
			interceptorsGeneric_once.erase(handlerIter);
			handler(std::move(resultGRMessage));
		}
		else
		{
			return resultMessage;
		}
	}
	else
	{
		auto handlerIter = interceptors_once.find(resultMessage->typeId);
		if (handlerIter != interceptors_once.end())
		{
			auto handler = handlerIter->second;
			interceptors_once.erase(handlerIter);
			handler(std::move(resultMessage));
		}
		else
		{
			return resultMessage;
		}
	}
	return nullptr;	
}

void Network::interceptOnce(int16_t messageType, MessageHandler handler)
{
	if (interceptors_once.count(messageType) > 0)
		THROW_FATAL_ERROR("message type already intercepted");
	if (messageType == MessageTypes::TYPE_REQUEST_MSG)
		THROW_FATAL_ERROR("can't intercept generic requests");
	interceptors_once[messageType] = handler;
}

void Network::interceptGenericRequestOnce(int16_t requestType, GenericRequestHandler handler)
{
	if (interceptorsGeneric_once.count(requestType) > 0)
		THROW_FATAL_ERROR("request type already intercepted");
	interceptorsGeneric_once[requestType] = handler;
}

std::unique_ptr<NetworkPacket> Network::getNewPacket()
{
#ifdef __EMSCRIPTEN__
	return std::make_unique<SimplePacket>();
#else	
	return std::make_unique<NetworkPacket>();
#endif
}

