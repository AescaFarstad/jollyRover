#pragma once
#include <cstdint>
#include <ISerializable.h>

enum class MESSAGE_TYPE : int16_t
{
	TYPE_UNINITIALIZED = -1,
	TYPE_REQUEST_MSG = 1,
	TYPE_GREETING_MSG = 2,
	TYPE_INPUT_ACTION_MSG = 3,
	TYPE_INPUT_JOINED_MSG = 4,
	TYPE_INPUT_LEFT_MSG = 5,
	TYPE_GAME_STATE_MSG = 6,
	TYPE_INPUT_ROUTE_MSG = 7,
	TYPE_INPUT_TIME_MSG = 8,
	TYPE_LOAD_GAME_MSG = 9,
	TYPE_INPUT_IMPULSE_MSG = 10
};

enum class REQUEST_TYPE : int16_t
{
	REQUEST_GREETING = 1,
	REQUEST_JOIN_GAME = 2,
	REQUEST_GAME_STATE = 3,
	REQUEST_PING = 4,
	REQUEST_PONG = 5
};

namespace Serializer {
	
	void write(const MESSAGE_TYPE& value, SerializationStream& stream);
	void read(MESSAGE_TYPE& value, SerializationStream& stream);
	void write(const REQUEST_TYPE& value, SerializationStream& stream);
	void read(REQUEST_TYPE& value, SerializationStream& stream);
}