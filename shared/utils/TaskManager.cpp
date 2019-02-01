#include <TaskManager.h>

TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
	for (Task* task : tasks)
	{
		delete task;
	}
}

void TaskManager::push(std::unique_ptr<Task> task)
{
	S::log.add("Task [" + task->getName() + "] is added", { LOG_TAGS::TASK });
	tasks.push_back(task.release());
}

void TaskManager::update()
{
	for (Task* &task : tasks)
	{
		task->update();
	}

	for (Task* &task : tasks)
	{
		if (task->isComplete())
		{
			S::log.add("Task [" + task->getName() + "] is complete", { LOG_TAGS::TASK });
			auto i = &task - &tasks[0];
			delete task;
			tasks[i] = nullptr;
		}
	}

	tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](Task* task) {
		return task == nullptr;
	}), tasks.end());
}

