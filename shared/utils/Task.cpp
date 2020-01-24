#include <Task.h>

size_t Task::idCounter = 1000;

Task::Task()
{
	_isComplete = false;
	id = idCounter++;
}

bool Task::isComplete()
{
	return _isComplete;
}

void Task::update()
{
	_isComplete = true;
}

std::string Task::getName()
{
	return "Anonymous task #" + std::to_string(id);
}
