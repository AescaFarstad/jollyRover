#pragma once
#include <Task.h>
#include <vector>
#include <memory>

class TaskManager
{
public:
	void push(Task* task);
	void abort(Task* task);
	void update();

private:
	std::vector<Task*> tasks;
};
