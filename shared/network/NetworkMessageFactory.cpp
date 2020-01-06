#include <NetworkMessageFactory.h>
#include <MessageTypes.h>
#include <InputMessage.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <InputRouteMessage.h>
#include <InputTimeMessage.h>
#include <InputActionMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <InputImpulseMessage.h>
#include <InputDebugMessage.h>
#include <LoadGameMessage.h>
#include <ReadOnlySerializationStream.h>
#include <GameStateMessage.h>

std::unique_ptr<NetworkMessage> NetworkMessageFactory::parse(const NetworkPacket& packet)
{
	int16_t type;
	Serializer::read(type, packet.payload);
	ReadOnlySerializationStream rstream(packet.payload, packet.payloadSize);
	std::unique_ptr<NetworkMessage> p = pointerByType((MESSAGE_TYPE)type);
	p->deserialize(rstream);
	return p;
}

std::unique_ptr<NetworkMessage> NetworkMessageFactory::pointerByType(MESSAGE_TYPE type)
{
	if (type == MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG)
		return std::make_unique<InputActionMessage>();
	else if (type == MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG)
		return std::make_unique<InputPlayerJoinedMessage>();
	else if (type == MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG)
		return std::make_unique<InputPlayerLeftMessage>();
	else if (type == MESSAGE_TYPE::TYPE_REQUEST_MSG)
		return std::make_unique<GenericRequestMessage>();
	else if (type == MESSAGE_TYPE::TYPE_GREETING_MSG)
		return std::make_unique<GreetingMessage>();
	else if (type == MESSAGE_TYPE::TYPE_GAME_STATE_MSG)
		return std::make_unique<GameStateMessage>();
	else if (type == MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG)
		return std::make_unique<InputRouteMessage>();
	else if (type == MESSAGE_TYPE::TYPE_INPUT_TIME_MSG)
		return std::make_unique<InputTimeMessage>();
	else if (type == MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG)
		return std::make_unique<InputImpulseMessage>();
	else if (type == MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG)
		return std::make_unique<InputDebugMessage>();
	else if (type == MESSAGE_TYPE::TYPE_LOAD_GAME_MSG)
		return std::make_unique<LoadGameMessage>();
	else
		THROW_FATAL_ERROR("Unknow Network Message " + std::to_string(type));
	return nullptr;
}