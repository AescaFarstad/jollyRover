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
	Logger(std::initializer_list<LOG_TAGS> enabledTags, std::initializer_list<LOG_TAGS> disabledTags);
	~Logger();

	void add(std::string message, std::initializer_list<LOG_TAGS> tags);
	void add(std::string message);
	void enableTags(std::initializer_list<LOG_TAGS> enabledTags);
	void disableTags(std::initializer_list<LOG_TAGS> disabledtags);

private:
	std::vector<LogMessage> messages;
	std::vector<LOG_TAGS> enabledTags;
	std::vector<LOG_TAGS> disabledTags;

	bool isReportable(std::initializer_list<LOG_TAGS> tags);
	void report(LogMessage* msg);
};