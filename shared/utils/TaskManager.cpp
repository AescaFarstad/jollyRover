#include <TaskManager.h>
#include <Global.h>

void TaskManager::push(Task* task)
{
	S::log.add("Task [" + task->getName() + "] is added", { LOG_TAGS::TASK });
	tasks.push_back(std::move(task));
}

void TaskManager::abort(Task* task)
{
	task->abort();
	tasks.erase(std::remove(tasks.begin(), tasks.end(), task));
}

void TaskManager::update()
{
	for (auto& task : tasks)
	{
		task->update();
	}

	for (auto& task : tasks)
	{
		if (task->isComplete())
		{
			S::log.add("Task [" + task->getName() + "] is complete", { LOG_TAGS::TASK });
			task = nullptr;
		}
	}

	tasks.erase(std::remove_if(tasks.begin(), tasks.end(), [](const auto& task) {
		return task == nullptr;
	}), tasks.end());
}

