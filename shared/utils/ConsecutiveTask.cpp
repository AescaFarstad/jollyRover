#include <ConsecutiveTask.h>

ConsecutiveTask::ConsecutiveTask()
{
	reset();
}

ConsecutiveTask::~ConsecutiveTask()
{
	if (m_isRunning)
		THROW_FATAL_ERROR("DELETING RUNNING TASK");
}

void ConsecutiveTask::exec()
{
	m_isStarted = true;
}

void ConsecutiveTask::reset()
{
	m_currentSubTask = 0;
	m_isWaitingForCallback = false;
	m_isRunning = false;
	m_isStarted = false;
	m_isComplete = false;
	m_isWaitingForRepeatedFunction = false;
	onAbort = 0;
	m_pendingCallback.reset();
	m_subTasks.clear();
}

void ConsecutiveTask::abort()
{
	S::log.add(getName() + "-> aborting at #" + std::to_string(m_currentSubTask) + ": " + m_subTasks[m_currentSubTask].name, 
		{ LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
	if (onAbort)
		onAbort();
	reset();
}

void ConsecutiveTask::update()
{
	if (!m_isStarted || m_isComplete)
		return;
	if (m_subTasks.size() <= m_currentSubTask)
	{
		m_isComplete = true;
		return;
	}

	m_isRunning = true;
	while (
			!m_isWaitingForCallback && 
			!m_isWaitingForRepeatedFunction && 
			m_subTasks.size() > m_currentSubTask
		)
	{
		if (m_currentSubTask == 0)
			S::log.add(getName() + "-> starting #" + std::to_string(m_currentSubTask) + ": " + m_subTasks[m_currentSubTask].name, { LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
		tryToAdvance();
	}
	m_isWaitingForRepeatedFunction = false;
	m_isRunning = false;
}

void ConsecutiveTask::tryToAdvance()
{
	switch (m_subTasks[m_currentSubTask].type)
	{
		case SUB_TASK_TYPE::EMPTY:
		{
			THROW_FATAL_ERROR("EMPTY sub task");
			return;
		}
		case SUB_TASK_TYPE::SYNC:
		{
			m_subTasks[m_currentSubTask].syncFunction();
			onTaskComplete();
			return;
		}
		case SUB_TASK_TYPE::ASYNC:
		{
			std::unique_ptr<Callback> callback = std::make_unique<Callback>([this]() { this->onTaskComplete(); });
			m_pendingCallback = callback->createPendingCallback();
			m_isWaitingForCallback = true;
			m_subTasks[m_currentSubTask].asyncFunction(std::move(callback));
			return;
		}
		case SUB_TASK_TYPE::REPEATED:
		{
			if (m_subTasks[m_currentSubTask].repeatedFunction())
				onTaskComplete();
			else
				m_isWaitingForRepeatedFunction = true;
			return;
		}
		case SUB_TASK_TYPE::SUB_TASK:
		{
			m_subTasks[m_currentSubTask].subTask->update();
			if (m_subTasks[m_currentSubTask].subTask->isComplete())
				onTaskComplete();
			else
				m_isWaitingForRepeatedFunction = true;
			return;
		}
	}
}

void ConsecutiveTask::onTaskComplete()
{
	m_isWaitingForCallback = false;
	m_currentSubTask++;
	if (m_subTasks.size() > m_currentSubTask)
		S::log.add(getName() + "-> starting #" + std::to_string(m_currentSubTask) + ": " + m_subTasks[m_currentSubTask].name, { LOG_TAGS::TASK, LOG_TAGS::SUBTASK });
}
std::string ConsecutiveTask::getName()
{
	return"ConsecutiveTask #" + std::to_string(m_id) + " (" + std::to_string(m_subTasks.size()) + " elements)";
}

void ConsecutiveTask::pushSync(std::function<void()> syncFunction, std::string name)
{
	if (m_isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::SYNC;
	newTask.syncFunction = syncFunction;
	m_subTasks.push_back(newTask);
}

void ConsecutiveTask::pushAsync(std::function<void(std::unique_ptr<Callback>)> asyncFunction, std::string name)
{
	if (m_isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::ASYNC;
	newTask.asyncFunction = asyncFunction;
	m_subTasks.push_back(newTask);
}

void ConsecutiveTask::pushRepeated(std::function<bool()> repeatedFunction, std::string name)
{
	if (m_isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = name;
	newTask.type = SUB_TASK_TYPE::REPEATED;
	newTask.repeatedFunction = repeatedFunction;
	m_subTasks.push_back(newTask);
}

void ConsecutiveTask::pushSubTask(Task* subTask)
{
	if (m_isStarted)
		THROW_FATAL_ERROR("Can insert into a running task");

	ConsecutiveSubTask newTask;
	newTask.name = subTask->getName();
	newTask.type = SUB_TASK_TYPE::SUB_TASK;
	newTask.subTask = subTask;
	m_subTasks.push_back(newTask);
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
