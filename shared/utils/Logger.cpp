#include "Logger.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

Logger::Logger(std::initializer_list<LOG_TAGS> enabledTags, std::initializer_list<LOG_TAGS> disabledTags)
{
	enableTags(enabledTags);
	disableTags(disabledTags);
}

void Logger::add(const std::string& message, std::initializer_list<LOG_TAGS> tags)
{
	m_messages.push_back(LogMessage{SDL_GetTicks(), message});

	if (isReportable(tags))
		report(m_messages.back());
}

void Logger::add(const std::string& message)
{
	add(message, { LOG_TAGS::ANONYMOS });
}

void Logger::enableTags(std::initializer_list<LOG_TAGS> enabledTags)
{
	this->m_enabledTags.insert(this->m_enabledTags.end(), enabledTags.begin(), enabledTags.end());
	for (LOG_TAGS i : m_enabledTags)
	{
		auto toDelete = std::find(m_disabledTags.begin(), m_disabledTags.end(), i);
		if (toDelete != m_disabledTags.end())
			this->m_disabledTags.erase(toDelete);
	}
}

void Logger::disableTags(std::initializer_list<LOG_TAGS> disabledTags)
{
	this->m_disabledTags.insert(this->m_disabledTags.end(), disabledTags.begin(), disabledTags.end());
	for (LOG_TAGS i : m_disabledTags)
	{
		auto toDelete = std::find(m_enabledTags.begin(), m_enabledTags.end(), i);
		if (toDelete != m_enabledTags.end())
			this->m_enabledTags.erase(toDelete);
	}
}

bool Logger::isReportable(const std::initializer_list<LOG_TAGS>& tags)
{
	bool accept = false;
	bool reject = false;

	for (LOG_TAGS i : tags)
	{
		if (!accept && std::find(m_enabledTags.begin(), m_enabledTags.end(), i) != m_enabledTags.end())
			accept = true;
		if (!reject && std::find(m_disabledTags.begin(), m_disabledTags.end(), i) != m_disabledTags.end())
			reject = true;
	}
	return accept || !reject;
}

void Logger::report(LogMessage& msg)
{
	std::cout << std::setfill(' ') << std::setw(6) << msg.stamp << " " << msg.message << std::endl;
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

