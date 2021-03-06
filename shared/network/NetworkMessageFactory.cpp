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
#include <DemoDataMessage.h>
#include <DemoListMessage.h>
#include <DemoRequestMessage.h>
#include <GameStateMessage.h>
#include <ChecksumMessage.h>
#include <StateRequestMessage.h>
#include <HeartbeatMessage.h>
#include <SerializeSimpleTypes.h>

std::unique_ptr<NetworkMessage> NetworkMessageFactory::parse(const char* data, size_t size)
{
	BinarySerializer bs(data, size);
	return parse(bs);
}

std::unique_ptr<NetworkMessage> NetworkMessageFactory::parse(BinarySerializer& serializer)
{
	int16_t type;
	serializer.peek(type);
	serializer.peek(type);
	
	switch ((MESSAGE_TYPE)type)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:	{auto result =std::make_unique<InputActionMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:	{auto result =std::make_unique<InputPlayerJoinedMessage>(); result->deserialize(serializer); return result;}case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG: 	{auto result =std::make_unique<InputPlayerLeftMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_REQUEST_MSG: 		{auto result =std::make_unique<GenericRequestMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_GREETING_MSG: 		{auto result =std::make_unique<GreetingMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_GAME_STATE_MSG: 	{auto result =std::make_unique<GameStateMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG: 	{auto result =std::make_unique<InputRouteMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG: 	{auto result =std::make_unique<InputTimeMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG: 	{auto result =std::make_unique<InputImpulseMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG: 	{auto result =std::make_unique<InputDebugMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG: 		{auto result =std::make_unique<LoadGameMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_HEARTBEAT_MSG: 		{auto result =std::make_unique<HeartbeatMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_CHECKSUM_MSG: 		{auto result =std::make_unique<ChecksumMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_STATE_REQUEST_MSG:	{auto result =std::make_unique<StateRequestMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_DEMO_LIST:			{auto result =std::make_unique<DemoListMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_DEMO_REQUEST:		{auto result =std::make_unique<DemoRequestMessage>(); result->deserialize(serializer); return result;}
		case MESSAGE_TYPE::TYPE_DEMO_DATA:			{auto result =std::make_unique<DemoDataMessage>(); result->deserialize(serializer); return result;}
		default: THROW_FATAL_ERROR("Unknow Network Message " + std::to_string(type));
			break;
	}
}
