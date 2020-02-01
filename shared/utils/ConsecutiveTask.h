#pragma once
#include <Task.h>
#include <Global.h>
#include <Callback.h>
#include <functional>
#include <vector>
#include <memory>



class ConsecutiveTask : public Task
{
public:
	ConsecutiveTask();
	~ConsecutiveTask();

	void abort() override;
	void update() override;
	std::string getName() override;


	void pushSync(std::function<void()> syncFunction, std::string name = "noname");
	void pushAsync(std::function<void(std::unique_ptr<Callback>)> asyncFunction, std::string name = "noname");
	void pushRepeated(std::function<bool()> repeatedFunction, std::string name = "noname");
	void pushSubTask(Task* subTask);
	void onTaskComplete();
	void exec();
	void reset();

private:
	bool m_isStarted;
	bool m_isRunning;
	bool m_isWaitingForCallback;
	bool m_isWaitingForRepeatedFunction;
	size_t m_currentSubTask;

	void tryToAdvance();
	std::unique_ptr<PendingCallback> m_pendingCallback;

	enum SUB_TASK_TYPE
	{
		EMPTY,
		SYNC,
		ASYNC,
		REPEATED,
		SUB_TASK
	};

	struct ConsecutiveSubTask
	{
		ConsecutiveSubTask();
		~ConsecutiveSubTask();

		SUB_TASK_TYPE type;
		std::string name;

		std::function<void()> syncFunction;
		std::function<void(std::unique_ptr<Callback>)> asyncFunction;
		std::function<bool()> repeatedFunction;
		Task* subTask;
	}; 
	std::vector<ConsecutiveSubTask> m_subTasks;
};
