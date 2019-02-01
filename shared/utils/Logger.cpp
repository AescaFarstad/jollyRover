#include "Logger.h"

Logger::Logger()
{
}

Logger::Logger(std::initializer_list<LOG_TAGS> enabledTags, std::initializer_list<LOG_TAGS> disabledTags)
{
	enableTags(enabledTags);
	disableTags(disabledTags);
}

Logger::~Logger()
{
}

void Logger::add(std::string message, std::initializer_list<LOG_TAGS> tags)
{
	LogMessage* msg = new LogMessage();
	msg->message = message;
	msg->stamp = SDL_GetTicks();
	messages.push_back(*msg);

	if (isReportable(tags))
		report(msg);
}

void Logger::add(std::string message)
{
	add(message, { LOG_TAGS::ANONYMOS });
}

void Logger::enableTags(std::initializer_list<LOG_TAGS> enabledTags)
{
	this->enabledTags.insert(this->enabledTags.end(), enabledTags.begin(), enabledTags.end());
	for (LOG_TAGS i : enabledTags)
	{
		auto toDelete = std::find(disabledTags.begin(), disabledTags.end(), i);
		if (toDelete != disabledTags.end())
			this->disabledTags.erase(toDelete);
	}
}

void Logger::disableTags(std::initializer_list<LOG_TAGS> disabledTags)
{
	this->disabledTags.insert(this->disabledTags.end(), disabledTags.begin(), disabledTags.end());
	for (LOG_TAGS i : disabledTags)
	{
		auto toDelete = std::find(enabledTags.begin(), enabledTags.end(), i);
		if (toDelete != enabledTags.end())
			this->enabledTags.erase(toDelete);
	}
}

bool Logger::isReportable(std::initializer_list<LOG_TAGS> tags)
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

void Logger::report(LogMessage* msg)
{
	std::cout << std::setfill(' ') << std::setw(6) << msg->stamp << " " << msg->message << std::endl;
}
