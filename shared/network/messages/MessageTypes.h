#pragma once
#include <cstdint>
enum class MessageTypes : int16_t
{
	TYPE_UNINITIALIZED = -1,
	TYPE_REQUEST_MSG = 1,
	TYPE_GREETING_MSG = 2,
	TYPE_INPUT_ACTION_MSG = 3,
	TYPE_INPUT_JOINED_MSG = 4,
	TYPE_INPUT_LEFT_MSG = 5,
	TYPE_GAME_STATE_MSG = 6,
	TYPE_INPUT_ROUTE_MSG = 7,
	TYPE_INPUT_TIME_MSG = 8
};

enum class RequestTypes : int16_t
{
	REQUEST_GREETING = 1,
	REQUEST_JOIN_GAME = 2,
	REQUEST_GAME_STATE = 3,
	REQUEST_PING = 4,
	REQUEST_PONG = 5
};