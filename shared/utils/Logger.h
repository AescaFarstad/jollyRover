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

enum class LOG_TAGS : int8_t
{
	UNIQUE,
	ERROR_,
	ANONYMOS,

	NET,
	GAME,
	TASK,
	INPUT_,
	FILE_SYSTEM,


	NET_BRIEF,
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

	void add(const std::string& message, std::initializer_list<LOG_TAGS> tags);
	void add(const std::string& message);
	void enableTags(std::initializer_list<LOG_TAGS> m_enabledTags);
	void disableTags(std::initializer_list<LOG_TAGS> disabledtags);
	void toggleTag(LOG_TAGS tag);
	bool isEnabled(LOG_TAGS tag);

private:
	std::vector<LogMessage> m_messages;
	std::vector<LOG_TAGS> m_enabledTags;
	std::vector<LOG_TAGS> m_disabledTags;

	bool isReportable(std::initializer_list<LOG_TAGS> tags);
	void report(LogMessage* msg);
};