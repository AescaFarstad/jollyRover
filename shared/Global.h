#pragma once
#include <cstdint>
#include <Logger.h>
#include <cassert>

#define THROW_FATAL_ERROR(ERROR_MSG) {printf( "FATAL ERROR: \n%s @ %d \n%s \n", __FILE__, __LINE__, #ERROR_MSG); assert(0);}

namespace GAME_CONFIG
{

#ifdef __EMSCRIPTEN__
	const int port = 12880;
	const bool IS_WEB = true;
#else
	const int port = 12745;
	const bool IS_WEB = false;
#endif

	const char host[13] = "192.168.1.52";
	const int maxMessageSize = 30000;
	const int32_t simpleClientCode = 0xfcaceb;
	const int32_t logicStepSize = 10;
	const int32_t saveStateInterval = 3;
	const int32_t networkUpdateInterval = 5;
}

namespace S
{
	extern Logger log;
	int32_t getId();
	extern int32_t idCounter;
}