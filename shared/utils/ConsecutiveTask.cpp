#include <ConsecutiveTask.h>

ConsecutiveTask::ConsecutiveTask()
{
	currentSubTask = 0;
	isWaitingForCallback = false;
	isRunning = false;
	isStarted = false;
	isWaitingForRepeatedFunction = false;
}

ConsecutiveTask::~ConsecutiveTask()
{
	if (isRunning)
		THROW_FATAL_ERROR("DELETING RUNNING TASK");
}

void ConsecutiveTask::exec()
{
	isStarted = true;
}

void ConsecutiveTask::update()
{
	if (!isStarted || _isComplete)
		return;
	if (subTasks.size() <= currentSubTask)
	{
		_isComplete = true;
		return;
	}

	isRunning = true;
	while (
			!isWaitingForCallback && 
			!isWaitingForRepeatedFunction && 
			subTasks.size() > currentSubTask
		)
	{
		if (currentSubTask == 0)
			S::log.add(getName() + "-> starting #" + std::to_string(currentSubTask) + ": " + subTasks[currentSubTask].name, { LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
		tryToAdvance();
	}
	isWaitingForRepeatedFunction = false;
	isRunning = false;
}

void ConsecutiveTask::tryToAdvance()
{
	switch (subTasks[currentSubTask].type)
	{
		case SUB_TASK_TYPE::EMPTY:
		{
			THROW_FATAL_ERROR("EMPTY sub task");
			return;
		}
		case SUB_TASK_TYPE::SYNC:
		{
			subTasks[currentSubTask].syncFunction();
			onTaskComplete();
			return;
		}
		case SUB_TASK_TYPE::ASYNC:
		{
			std::unique_ptr<Callback> callback = std::make_unique<Callback>([this]() { this->onTaskComplete(); });
			pendingCallback = callback->createPendingCallback();
			isWaitingForCallback = true;
			subTasks[currentSubTask].asyncFunction(std::move(callback));
			return;
		}
		case SUB_TASK_TYPE::REPEATED:
		{
			if (subTasks[currentSubTask].repeatedFunction())
				onTaskComplete();
			else
				isWaitingForRepeatedFunction = true;
			return;
		}
		case SUB_TASK_TYPE::SUB_TASK:
		{
			subTasks[currentSubTask].subTask->update();
			if (subTasks[currentSubTask].subTask->isComplete())
				onTaskComplete();
			else
				isWaitingForRepeatedFunction = true;
			return;
		}
	}
}

void ConsecutiveTask::onTaskComplete()
{
	isWaitingForCallback = false;
	currentSubTask++;
	if (subTasks.size() > currentSubTask)
		S::log.add(getName() + "-> starting #" + std::to_string(currentSubTask) + ": " + subTasks[currentSubTask].name, { LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
}
std::string ConsecutiveTask::getName()
{
	return"ConsecutiveTask #" + std::to_string(id) + " (" + std::to_string(subTasks.size()) + " elements)";
}

void ConsecutiveTask::pushSync(std::function<void()> syncFunction, std::string name)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::SYNC;
	newTask.syncFunction = syncFunction;
	subTasks.push_back(newTask);
}

void ConsecutiveTask::pushAsync(std::function<void(std::unique_ptr<Callback>)> asyncFunction, std::string name)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::ASYNC;
	newTask.asyncFunction = asyncFunction;
	subTasks.push_back(newTask);
}

void ConsecutiveTask::pushRepeated(std::function<bool()> repeatedFunction, std::string name)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::REPEATED;
	newTask.repeatedFunction = repeatedFunction;
	subTasks.push_back(newTask);
}

void ConsecutiveTask::pushSubTask(Task* subTask)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = subTask->getName();
	newTask.type = SUB_TASK_TYPE::SUB_TASK;
	newTask.subTask = subTask;
	subTasks.push_back(newTask);
}



ConsecutiveTask::ConsecutiveSubTask::ConsecutiveSubTask()
{
	type = SUB_TASK_TYPE::EMPTY;
	subTask = nullptr;
}

ConsecutiveTask::ConsecutiveSubTask::~ConsecutiveSubTask()
{
	if (subTask != nullptr)
		delete subTask;
}
