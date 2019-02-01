#include <ConsequtiveTask.h>

ConsequtiveTask::ConsequtiveTask()
{
	currentSubTask = 0;
	isWaitingForCallback = false;
	isRunning = false;
	isStarted = false;
	isWaitingForRepeatedFunction = false;
}

ConsequtiveTask::~ConsequtiveTask()
{
	if (isRunning)
		THROW_FATAL_ERROR("DELETING RUNNING TASK");
}

void ConsequtiveTask::exec()
{
	isStarted = true;
}

void ConsequtiveTask::update()
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
			S::log.add(getName() + "-> starting: " + subTasks[currentSubTask].name, { LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
		tryToAdvance();
	}
	isWaitingForRepeatedFunction = false;
	isRunning = false;
}

void ConsequtiveTask::tryToAdvance()
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
			//std::function<void()> function = [this]() {/* this->onTaskComplete();*/ };
			//function();
			std::unique_ptr<Callback> callback = std::make_unique<Callback>([this]() { this->onTaskComplete(); });
			//callback->execute();
			pendingCallback = callback->createPendingCallback();
			isWaitingForCallback = true;
			//callback->execute();
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

void ConsequtiveTask::onTaskComplete()
{
	isWaitingForCallback = false;
	currentSubTask++;
	if (subTasks.size() > currentSubTask)
		S::log.add(getName() + "-> starting: " + subTasks[currentSubTask].name, { LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
}
std::string ConsequtiveTask::getName()
{
	return"ConsequtiveTask #" + std::to_string(id) + " (" + std::to_string(subTasks.size()) + " elements)";
}

void ConsequtiveTask::pushSync(std::function<void()> syncFunction, std::string name)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsequtiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::SYNC;
	newTask.syncFunction = syncFunction;
	subTasks.push_back(newTask);
}

void ConsequtiveTask::pushAsync(std::function<void(std::unique_ptr<Callback>)> asyncFunction, std::string name)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsequtiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::ASYNC;
	newTask.asyncFunction = asyncFunction;
	subTasks.push_back(newTask);
}

void ConsequtiveTask::pushRepeated(std::function<bool()> repeatedFunction, std::string name)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsequtiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::REPEATED;
	newTask.repeatedFunction = repeatedFunction;
	subTasks.push_back(newTask);
}

void ConsequtiveTask::pushSubTask(Task* subTask)
{
	if (isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsequtiveSubTask newTask;
	newTask.name = subTask->getName();
	newTask.type = SUB_TASK_TYPE::SUB_TASK;
	newTask.subTask = subTask;
	subTasks.push_back(newTask);
}



ConsequtiveTask::ConsequtiveSubTask::ConsequtiveSubTask()
{
	type = SUB_TASK_TYPE::EMPTY;
	subTask = nullptr;
}

ConsequtiveTask::ConsequtiveSubTask::~ConsequtiveSubTask()
{
	if (subTask != nullptr)
		delete subTask;
}
