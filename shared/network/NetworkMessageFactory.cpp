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
#include <HeartbeatMessage.h>

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
	switch (type)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:		return std::make_unique<InputActionMessage>();
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:		return std::make_unique<InputPlayerJoinedMessage>();
		case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG: 		return std::make_unique<InputPlayerLeftMessage>();
		case MESSAGE_TYPE::TYPE_REQUEST_MSG: 			return std::make_unique<GenericRequestMessage>();
		case MESSAGE_TYPE::TYPE_GREETING_MSG: 			return std::make_unique<GreetingMessage>();
		case MESSAGE_TYPE::TYPE_GAME_STATE_MSG: 		return std::make_unique<GameStateMessage>();
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG: 		return std::make_unique<InputRouteMessage>();
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG: 		return std::make_unique<InputTimeMessage>();
		case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG: 		return std::make_unique<InputImpulseMessage>();
		case MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG: 		return std::make_unique<InputDebugMessage>();
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG: 			return std::make_unique<LoadGameMessage>();
		case MESSAGE_TYPE::TYPE_HEARTBEAT_MSG: 			return std::make_unique<HeartbeatMessage>();
		default: THROW_FATAL_ERROR("Unknow Network Message " + std::to_string(type));
			break;
	}
	return nullptr;
}