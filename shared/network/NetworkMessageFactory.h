#pragma once
#include <memory>
#include <NetworkMessage.h>
#include <NetworkPacket.h>
#include <MessageTypes.h>
#include <InputMessage.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <InputRouteMessage.h>
#include <InputTimeMessage.h>
#include <InputActionMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <LoadGameMessage.h>
#include <ReadOnlySerializationStream.h>
#include <GameStateMessage.h>

class NetworkMessageFactory
{
public:
	NetworkMessageFactory();
	~NetworkMessageFactory();

	std::unique_ptr<NetworkMessage> parse(NetworkPacket* packet);

	std::unique_ptr<NetworkMessage> pointerByType(MESSAGE_TYPE type);
	
/*
	template <typename T, typename A>
	T pointerByType2(MESSAGE_TYPE type, A &source)
	{
		if (type == MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG)
			return std::make_unique<InputActionMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG)
			return std::make_unique<InputPlayerJoinedMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG)
			return std::make_unique<InputPlayerLeftMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_REQUEST_MSG)
			return std::make_unique<GenericRequestMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_GREETING_MSG)
			return std::make_unique<GreetingMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_GAME_STATE_MSG)
			return std::make_unique<GameStateMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_INPUT_TIME_MSG)
			return std::make_unique<InputTimeMessage>(source);
		else if (type == MESSAGE_TYPE::TYPE_LOAD_GAME_MSG)
			return std::make_unique<LoadGameMessage>(source);
		return nullptr;
	}
*/
};