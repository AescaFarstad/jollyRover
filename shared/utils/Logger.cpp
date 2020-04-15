#include <Logger.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

Logger::Logger(std::vector<LOG_TAGS> enabledTags, std::vector<LOG_TAGS> disabledTags, std::function<void(uint32_t, const std::string)> handleHardLog)
{
	enableTags(enabledTags);
	disableTags(disabledTags);
	m_handleHardLog = handleHardLog;
}

void Logger::add(const std::string& message, std::initializer_list<LOG_TAGS> tags)
{
	auto ticks = SDL_GetTicks();
	if (isReportable(tags))
		report(ticks, message);
	if (m_handleHardLog && std::find(tags.begin(), tags.end(), LOG_TAGS::HARD_LOG) != tags.end())
		m_handleHardLog(ticks, message);
}

void Logger::add(const std::string& message)
{
	add(message, { LOG_TAGS::ANONYMOS });
}

void Logger::enableTags(std::vector<LOG_TAGS> enabledTags)
{
	this->enabledTags.insert(this->enabledTags.end(), enabledTags.begin(), enabledTags.end());
	for (LOG_TAGS i : enabledTags)
	{
		auto toDelete = std::find(disabledTags.begin(), disabledTags.end(), i);
		if (toDelete != disabledTags.end())
			this->disabledTags.erase(toDelete);
	}
}

void Logger::disableTags(std::vector<LOG_TAGS> disabledTags)
{
	this->disabledTags.insert(this->disabledTags.end(), disabledTags.begin(), disabledTags.end());
	for (LOG_TAGS i : disabledTags)
	{
		auto toDelete = std::find(enabledTags.begin(), enabledTags.end(), i);
		if (toDelete != enabledTags.end())
			this->enabledTags.erase(toDelete);
	}
}

bool Logger::isReportable(const std::initializer_list<LOG_TAGS>& tags)
{
	bool accept = false;
	bool reject = false;

	for (LOG_TAGS i : tags)
	{
		if (!accept && std::find(enabledTags.begin(), enabledTags.end(), i) != enabledTags.end())
			accept = true;
		if (!reject && std::find(disabledTags.begin(), disabledTags.end(), i) != disabledTags.end())
			reject = true;
	}
	return accept || !reject;
}

void Logger::report(uint32_t ticks, std::string message)
{
	std::cout << std::setfill(' ') << std::setw(6) << ticks << " " << message << std::endl;
}

void Logger::toggleTag(LOG_TAGS tag)
{
	if (isEnabled(tag))
		disableTags({tag});
	else
		enableTags({tag});		
}

bool Logger::isEnabled(LOG_TAGS tag)
{
	return isReportable({tag});
}

