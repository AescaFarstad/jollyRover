#pragma once
#include <string>

class Task
{
public:
	Task();
	virtual ~Task() = default;

	bool isComplete();
	virtual void update();
	virtual void abort();
	virtual std::string getName();

protected:
	bool m_isComplete;
	size_t m_id;
	static size_t s_idCounter;
};