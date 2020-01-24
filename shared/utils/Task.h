#pragma once
#include <string>

class Task
{
public:
	Task();
	virtual ~Task() = default;

	bool isComplete();
	virtual void update();
	virtual std::string getName();

protected:
	bool _isComplete;
	size_t id;
	static size_t idCounter;
};