#include <Task.h>

size_t Task::s_idCounter = 1000;

Task::Task()
{
	m_isComplete = false;
	m_id = s_idCounter++;
}

bool Task::isComplete()
{
	return m_isComplete;
}

void Task::update()
{
	m_isComplete = true;
}

void Task::abort()
{
	m_isComplete = false;
	if (onAbort)
		onAbort();
}

std::string Task::getName()
{
	return "Anonymous task #" + std::to_string(m_id);
}
