#include <NetworkMessageFactory.h>

NetworkMessageFactory::NetworkMessageFactory()
{
}

NetworkMessageFactory::~NetworkMessageFactory()
{
}

std::unique_ptr<NetworkMessage> NetworkMessageFactory::parse(NetworkPacket* packet)
{
	int16_t type;
	Serializer::read(type, packet->payload);
	ReadOnlySerializationStream rstream(packet->payload, packet->payloadSize);
	std::unique_ptr<NetworkMessage> p = pointerByType(type);
	p->deserialize(rstream);
	return p;
}


std::unique_ptr<NetworkMessage> NetworkMessageFactory::pointerByType(int16_t type)
{
	if (type == MessageTypes::TYPE_INPUT_ACTION_MSG)
		return std::make_unique<InputActionMessage>();
	else if (type == MessageTypes::TYPE_INPUT_JOINED_MSG)
		return std::make_unique<InputPlayerJoinedMessage>();
	else if (type == MessageTypes::TYPE_INPUT_LEFT_MSG)
		return std::make_unique<InputPlayerLeftMessage>();
	else if (type == MessageTypes::TYPE_REQUEST_MSG)
		return std::make_unique<GenericRequestMessage>();
	else if (type == MessageTypes::TYPE_GREETING_MSG)
		return std::make_unique<GreetingMessage>();
	else if (type == MessageTypes::TYPE_GAME_STATE_MSG)
		return std::make_unique<GameStateMessage>();
	else if (type == MessageTypes::TYPE_INPUT_ROUTE_MSG)
		return std::make_unique<InputRouteMessage>();
	else
		THROW_FATAL_ERROR("Unknow Network Message " + std::to_string(type));
	return nullptr;
}