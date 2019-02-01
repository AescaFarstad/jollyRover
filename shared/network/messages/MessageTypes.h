#pragma once
#include <cstdint>
namespace MessageTypes
{
	const int16_t TYPE_REQUEST_MSG = 1;
	const int16_t TYPE_GREETING_MSG = 2;
	const int16_t TYPE_INPUT_ACTION_MSG = 3;
	const int16_t TYPE_INPUT_JOINED_MSG = 4;
	const int16_t TYPE_INPUT_LEFT_MSG = 5;
	const int16_t TYPE_GAME_STATE_MSG = 6;
	const int16_t TYPE_INPUT_ROUTE_MSG = 7;
}

namespace RequestTypes
{
	const int16_t REQUEST_GREETING = 1;
	const int16_t REQUEST_JOIN_GAME = 2;
	const int16_t REQUEST_GAME_STATE = 3;
	const int16_t REQUEST_PING = 4;
	const int16_t REQUEST_PONG = 5;
}