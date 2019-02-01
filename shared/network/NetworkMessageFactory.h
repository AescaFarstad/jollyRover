#pragma once
#include <memory>
#include <NetworkMessage.h>
#include <NetworkPacket.h>
#include <MessageTypes.h>
#include <InputMessage.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <InputRouteMessage.h>
#include <InputActionMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <ReadOnlySerializationStream.h>
#include <GameStateMessage.h>

class NetworkMessageFactory
{
public:
	NetworkMessageFactory();
	~NetworkMessageFactory();

	std::unique_ptr<NetworkMessage> parse(NetworkPacket* packet);

	std::unique_ptr<NetworkMessage> pointerByType(int16_t type);
	

	template <typename T, typename A>
	T pointerByType2(int16_t type, A &source)
	{
		if (type == MessageTypes::TYPE_INPUT_ACTION_MSG)
			return std::make_unique<InputActionMessage>(source);
		else if (type == MessageTypes::TYPE_INPUT_JOINED_MSG)
			return std::make_unique<InputPlayerJoinedMessage>(source);
		else if (type == MessageTypes::TYPE_INPUT_LEFT_MSG)
			return std::make_unique<InputPlayerLeftMessage>(source);
		else if (type == MessageTypes::TYPE_REQUEST_MSG)
			return std::make_unique<GenericRequestMessage>(source);
		else if (type == MessageTypes::TYPE_GREETING_MSG)
			return std::make_unique<GreetingMessage>(source);
		else if (type == MessageTypes::TYPE_GAME_STATE_MSG)
			return std::make_unique<GameStateMessage>(source);
		return nullptr;
	}

};