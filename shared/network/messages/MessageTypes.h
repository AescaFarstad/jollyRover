#pragma once
#include <cstdint>

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
	TYPE_INPUT_IMPULSE_MSG = 10,
	TYPE_INPUT_DEBUG_MSG = 11,
	TYPE_HEARTBEAT_MSG = 12,
	TYPE_STATE_REQUEST_MSG = 13,
	TYPE_CHECKSUM_MSG = 14,
	TYPE_DEMO_REQUEST = 15,
	TYPE_DEMO_LIST = 16,
	TYPE_DEMO_DATA = 17
	
	
};

enum class REQUEST_TYPE : int16_t
{
	INVALID = 0,
	REQUEST_GREETING = 1,
	REQUEST_JOIN_GAME = 2,
	REQUEST_GAME_STATE = 3,
	REQUEST_PING = 4,
	REQUEST_PONG = 5,
	REQUEST_DEMO_LIST = 6
};

#include <Serialization.h>

namespace Serialization
{
	
	//MESSAGE_TYPE---------------------------------------------------
	
	template <typename T>
	void write(const MESSAGE_TYPE& object, T& serializer)
	{
		serializer.write((int16_t)object, FIELD_NAME(MESSAGE_TYPE));
	}
	
	template <typename T>
	void read(MESSAGE_TYPE& object, T& serializer)
	{
		int16_t tmp;
		serializer.read(tmp, FIELD_NAME(MESSAGE_TYPE));
		object = (MESSAGE_TYPE)tmp;
	}
	
	//REQUEST_TYPE---------------------------------------------------
	
	template <typename T>
	void write(const REQUEST_TYPE& object, T& serializer)
	{
		serializer.write((int16_t)object, FIELD_NAME(REQUEST_TYPE));
	}
	
	template <typename T>
	void read(REQUEST_TYPE& object, T& serializer)
	{
		int16_t tmp;
		serializer.read(tmp, FIELD_NAME(REQUEST_TYPE));
		object = (REQUEST_TYPE)tmp;
	}
}