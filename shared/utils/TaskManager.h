#pragma once
#include <Task.h>
#include <vector>
#include <Global.h>
#include <memory>

class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	void push(std::unique_ptr<Task> task);
	void update();

private:
	std::vector<Task*> tasks;
};

