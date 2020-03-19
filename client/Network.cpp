#include <Network.h>
#include <HeartbeatMessage.h>
#include <SerializeSimpleTypes.h>

namespace S
{
	Network* network;
}

Network::Network(VariableProto* vars)
{
	m_vars = vars;
	SDLNet_Init();
	m_socket = nullptr;
	
	m_packetReader = std::make_unique<PacketReader>(&m_socket, getNewPacket, [this]() {
		m_activeSockets = SDLNet_CheckSockets(m_socketSet, 0);
		if (m_activeSockets == -1)
		{
			S::log.add("SDLNet_CheckSockets: " + std::string(SDLNet_GetError()), { LOG_TAGS::NET, LOG_TAGS::ERROR_ });
			m_isConnected = false;
		}
		if (m_activeSockets <= 0)
			return 0;
		return SDLNet_SocketReady(m_socket); 
	});

	//----------------------------------

	auto handleGenericRequest = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message) {
		GenericRequestMessage* t = dynamic_cast<GenericRequestMessage*>(message.get());
		auto name = t->getName();
		auto request = t->request;
		if (!genericRequestBinder.process(std::move(message)))
		{
			S::log.add("GenericRequestMessage is not handled: " + name + " " + std::to_string((int16_t)request), { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF });
		}
	});
	
	std::unique_ptr<AnonymousBinding> binding = std::make_unique<AnonymousBinding>("Network generic binding");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_REQUEST_MSG)->
	setCallOnce(false)->
	setHandler(std::move(handleGenericRequest));
	binder.bind(std::move(binding));

	//----------------------------------
}

Network::~Network()
{
	if (m_socket != nullptr)
	{
		SDLNet_TCP_DelSocket(m_socketSet, m_socket);
		SDLNet_TCP_Close(m_socket);
	}
	SDLNet_FreeSocketSet(m_socketSet);
}

void Network::connect()
{
	if (m_isConnected || m_socket != nullptr)
		THROW_FATAL_ERROR("Attempt to connect while connected");
	
	IPaddress ip;
	SDLNet_ResolveHost(&ip, S::config.host, S::config.port);
	m_socket = SDLNet_TCP_Open(&ip);

	S::log.add("connect status: " + std::string(m_socket == nullptr ? "false" : "true"), {LOG_TAGS::NET, LOG_TAGS::NET_BRIEF});
	S::log.add("ip, port: " + std::string(S::config.host) + " " + std::to_string(S::config.port), {LOG_TAGS::NET, LOG_TAGS::NET_BRIEF});

	m_isConnected = m_socket != nullptr;
	if (m_isConnected)
	{
		SDLNet_TCP_AddSocket(m_socketSet, m_socket);
		
		if (!S::config.IS_WEB)
		{
			char codeMsg[4];
			Serializer::write(S::config.simpleClientCode, codeMsg);
			SDLNet_TCP_Send(m_socket, codeMsg, 4);
		}
		
		m_monitor.begin(SDL_GetTicks(), m_vars->heartbeatInterval, m_vars->heartbeatTimeout);
	}
}

bool Network::isConnected()
{
	return m_isConnected;
}

void Network::update()
{
	if (!m_isConnected)
		return;

	m_activeSockets = SDLNet_CheckSockets(m_socketSet, S::config.networkUpdateInterval);

	if (m_activeSockets == -1)
	{
		S::log.add("SDLNet_CheckSockets: " + std::string(SDLNet_GetError()), { LOG_TAGS::NET, LOG_TAGS::ERROR_ });
		m_isConnected = false;
		return;
	}

	std::unique_ptr<NetworkMessage> message = poll();
	while (message != nullptr)
	{
		std::string name = message->getName();
		if (!binder.process(std::move(message)))
		{
			S::log.add("NetworkMessage is not handled: " + name, { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF });
		}
		message = poll();
	}
	
	auto time = SDL_GetTicks();
	if (m_monitor.pollHeartbeat(time))
		send(HeartbeatMessage());
	if (m_monitor.isDisconnected(time))
		handleDisconnect();
}

void Network::send(const NetworkPacket& packet)
{
	if (m_socket == nullptr)
	{
		S::log.add("Message will not be sent, not connected.", { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF, LOG_TAGS::ERROR_ });
	}
	else
	{
		SDLNet_TCP_Send(m_socket, packet.rawData, packet.rawSize);
	}
}

void Network::send(const NetworkMessage& msg)
{
	std::unique_ptr<NetworkPacket> packet = getNewPacket();
	packet->setPayloadFromSerializable(msg);

	S::log.add("SEND " + msg.getName() + " " + std::to_string(packet->payloadSize) + ":\n\t" +
		Serializer::toHex(packet->payload, packet->payloadSize),
		{ LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });
	S::log.add("SEND " + msg.getName() + "[" + std::to_string(packet->payloadSize) + "]",
		{ LOG_TAGS::NET_BRIEF });

	m_requestTimeByInitiatorId[msg.initiator_id] = SDL_GetTicks();
	send(*packet);
}

std::unique_ptr<NetworkMessage> Network::poll()
{
	if (!m_isConnected || m_activeSockets <= 0)
		return nullptr;

	m_packetReader->setDataAvailable(SDLNet_SocketReady(m_socket));
	std::unique_ptr<NetworkPacket> packet = m_packetReader->poll();
	if (!packet)
	{
		return nullptr;
	}
	else if (packet->isDisconnectNotice)
	{
		handleDisconnect();
		return nullptr;
	}
	
	auto result = processIncomingPacket(std::move(packet));
	
	if (result)
		return result;
	else return poll();
}

void Network::handleDisconnect()
{
	m_isConnected = false;
	S::log.add("dm_isConnected",	{ LOG_TAGS::NET, LOG_TAGS::NET_BRIEF });
	SDLNet_TCP_DelSocket(m_socketSet, m_socket);
	SDLNet_TCP_Close(m_socket);
	m_socket = nullptr;
}

std::unique_ptr<NetworkMessage> Network::processIncomingPacket(std::unique_ptr<NetworkPacket> packet)
{
	S::log.add("incoming (" + std::to_string(packet->payloadSize) + "):\n\t" +
		Serializer::toHex(packet->payload, packet->payloadSize),
		{ LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });

	std::unique_ptr<NetworkMessage> resultMessage = factory.parse(*packet);

	//auto t = m_requestTimeByInitiatorId[resultMessage->inResponseTo];
	int32_t ticks = SDL_GetTicks();
	if (resultMessage->inResponseTo > 0 &&
		m_requestTimeByInitiatorId[resultMessage->inResponseTo] &&
		resultMessage->stamp != 0)
	{
		int32_t uncertaintyBefore = timeSync.getUncertainty();
		timeSync.addMeasurement(m_requestTimeByInitiatorId[resultMessage->inResponseTo], resultMessage->stamp, ticks);
		int32_t uncertaintyAfter = timeSync.getUncertainty();

		if (uncertaintyAfter < uncertaintyBefore || true)
			S::log.add("TimeSync range: " + std::to_string(uncertaintyBefore) + " -> " + std::to_string(uncertaintyAfter), { LOG_TAGS::NET });
	}

	S::log.add("RCVD " + resultMessage->getName(), { LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE, LOG_TAGS::NET_BRIEF });

	if (resultMessage->typeId == MESSAGE_TYPE::TYPE_REQUEST_MSG)
	{
		GenericRequestMessage* grM = static_cast<GenericRequestMessage*>(resultMessage.get());
		auto handlerIter = m_interceptorsGeneric_once.find(grM->request);
		if (handlerIter != m_interceptorsGeneric_once.end())
		{
			resultMessage.release();
			std::unique_ptr<GenericRequestMessage> resultGRMessage = std::unique_ptr<GenericRequestMessage>(grM);

			auto handler = handlerIter->second;
			m_interceptorsGeneric_once.erase(handlerIter);
			handler(std::move(resultGRMessage));
		}
		else
		{
			return resultMessage;
		}
	}
	else if (resultMessage->typeId == MESSAGE_TYPE::TYPE_HEARTBEAT_MSG)
	{
		m_monitor.onHearbeatMessage(ticks);
	}
	else
	{
		auto handlerIter = m_interceptors_once.find(resultMessage->typeId);
		if (handlerIter != m_interceptors_once.end())
		{
			auto handler = handlerIter->second;
			m_interceptors_once.erase(handlerIter);
			handler(std::move(resultMessage));
		}
		else
		{
			return resultMessage;
		}
	}
	return nullptr;	
}

void Network::interceptOnce(MESSAGE_TYPE messageType, MessageHandler handler)
{
	if (m_interceptors_once.count(messageType) > 0)
		THROW_FATAL_ERROR("message type already intercepted");
	if (messageType == MESSAGE_TYPE::TYPE_REQUEST_MSG)
		THROW_FATAL_ERROR("can't intercept generic requests");
	m_interceptors_once[messageType] = handler;
}

void Network::interceptGenericRequestOnce(REQUEST_TYPE requestType, GenericRequestHandler handler)
{
	if (m_interceptorsGeneric_once.count(requestType) > 0)
		THROW_FATAL_ERROR("request type already intercepted");
	m_interceptorsGeneric_once[requestType] = handler;
}


void Network::clearInterception(MESSAGE_TYPE messageType)
{
	m_interceptors_once.erase(messageType);
}

void Network::clearInterception(REQUEST_TYPE requestType)
{
	m_interceptorsGeneric_once.erase(requestType);	
}

std::unique_ptr<NetworkPacket> Network::getNewPacket()
{
#ifdef __EMSCRIPTEN__
	return std::make_unique<SimplePacket>();
#else	
	return std::make_unique<NetworkPacket>();
#endif
}

