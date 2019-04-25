#pragma once
#include <vector>
#include <cstdarg>
#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>

#ifdef __EMSCRIPTEN__
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
#endif

enum class LOG_TAGS : int
{
	UNIQUE,
	ERROR_,
	ANONYMOS,

	NET,
	GAME,
	TASK,
	INPUT_,
	FILE_SYSTEM,


	NET_MESSAGE,
	SUBTASK,

	Z
};

struct LogMessage
{
	int32_t stamp;
	std::string message;
};

class Logger
{
public:
	Logger();
	Logger(std::initializer_list<LOG_TAGS> m_enabledTags, std::initializer_list<LOG_TAGS> m_disabledTags);
	~Logger();

	void add(std::string message, std::initializer_list<LOG_TAGS> tags);
	void add(std::string message);
	void enableTags(std::initializer_list<LOG_TAGS> m_enabledTags);
	void disableTags(std::initializer_list<LOG_TAGS> disabledtags);

private:
	std::vector<LogMessage> m_messages;
	std::vector<LOG_TAGS> m_enabledTags;
	std::vector<LOG_TAGS> m_disabledTags;

	bool isReportable(std::initializer_list<LOG_TAGS> tags);
	void report(LogMessage* msg);
};