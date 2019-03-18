#include "TaskManager.h"
#include "Global.h"

void TaskManager::push(std::unique_ptr<Task> task)
{
	S::log.add("Task [" + task->getName() + "] is added", { LOG_TAGS::TASK });
	tasks.push_back(std::move(task));
}

void TaskManager::update()
{
	// TODO: Merge this loop with the next one?
	//       Or at least explain why it has been separated.
	for (auto && task : tasks)
	{
		task->update();
	}

	for (auto && task : tasks)
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

