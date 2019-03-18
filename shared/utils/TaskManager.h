#pragma once
#include <Task.h>
#include <vector>
#include <memory>

class TaskManager
{
public:
	void push(std::unique_ptr<Task> task);
	void update();

private:
	std::vector<std::unique_ptr<Task>> tasks;
};
