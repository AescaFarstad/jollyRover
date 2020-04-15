#pragma once
#include <vector>
#include <string>
#include <functional>

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
	HARD_LOG,


	NET_BRIEF,
	NET_MESSAGE,
	SUBTASK,

	Z
};

class Logger
{
public:
	Logger() = default;
	Logger(std::vector<LOG_TAGS> enabledTags, std::vector<LOG_TAGS> disabledTags, std::function<void(uint32_t, const std::string)> handleHardLog);

	void add(const std::string& message, std::initializer_list<LOG_TAGS> tags);
	void add(const std::string& message);
	void enableTags(std::vector<LOG_TAGS> enabledTags);
	void disableTags(std::vector<LOG_TAGS> disabledtags);
	void toggleTag(LOG_TAGS tag);
	bool isEnabled(LOG_TAGS tag);
	std::vector<LOG_TAGS> enabledTags;
	std::vector<LOG_TAGS> disabledTags;

private:
	std::function<void(uint32_t, const std::string)> m_handleHardLog;

	bool isReportable(const std::initializer_list<LOG_TAGS>& tags);
	void report(uint32_t ticks, std::string message);
};