#pragma once
#include <Global.h>
#include <functional>
#include <memory>

class PendingCallback;
class Callback
{
	friend PendingCallback;
public:
	Callback();
	Callback(std::function<void()> function);
	~Callback();
	Callback(const Callback& that) = delete;
	Callback& operator=(const Callback& that) = delete;
	Callback(Callback&& that);
	Callback& operator=(Callback&& that);

	void execute();
	std::unique_ptr<PendingCallback> createPendingCallback();
	int32_t id;

protected:
	PendingCallback* pendingCallback;
	bool isValid;
	std::function<void()> function;
};

class PendingCallback
{
	friend Callback;
public:
	PendingCallback();
	~PendingCallback();
	PendingCallback(PendingCallback&& that);
	PendingCallback& operator=(PendingCallback&& that);
	void disconnect();
	void cancel();

private:
	Callback* callback;

};

class DeathNotice : Callback
{
public:
	DeathNotice();
	~DeathNotice();
	DeathNotice(const DeathNotice& that) = delete;
	DeathNotice(DeathNotice&& that);
	std::unique_ptr<PendingCallback> subscribe(std::function<void()> function);

private:

};