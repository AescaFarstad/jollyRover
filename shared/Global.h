#pragma once
#include <cstdint>
#include <Logger.h>
#include <cassert>
#include <json.hpp>

#define THROW_FATAL_ERROR(ERROR_MSG) {printf( "FATAL ERROR: \n%s @ %d \n%s \n", __FILE__, __LINE__, #ERROR_MSG); abort();}

class GameConfig
{
public:
#ifdef __EMSCRIPTEN__
	const bool IS_WEB = true;
#else
	const bool IS_WEB = false;
#endif

#ifdef IS_SERVER
	const bool IS_CLIENT = false;
#else
	const bool IS_CLIENT = true;
#endif

	const int32_t simpleClientCode = 0xfcaceb;
	
	int32_t port;
	int32_t webPort;
	char host[17];
	int32_t maxMessageSize;
	uint32_t maxSaveStates;
	uint32_t saveStateInterval;
	int32_t networkUpdateInterval;
	bool loopBack;
	bool runBenchmark;
	int32_t window_X;
	int32_t window_Y;
	
	void load(nlohmann::json& j);
	
private:
	void loadLogTags(nlohmann::json& j);
	void setTagByName(std::string name, bool value);
	void setTagByTag(LOG_TAGS tag, bool value);
};

namespace S
{
	extern Logger log;
	int32_t getId();
	extern int32_t idCounter;
	extern GameConfig config;
};