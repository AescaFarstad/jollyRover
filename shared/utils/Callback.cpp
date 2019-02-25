#include <Callback.h>

Callback::Callback()
{
	pendingCallback = nullptr;
	isValid = false;
	id = S::getId();
}

Callback::Callback(std::function<void()> function)
{
	this->function = function;
	isValid = true;
	pendingCallback = nullptr;
}

Callback::~Callback()
{
	std::cout << "~Callback" + std::to_string(id) + "\n";
	if (pendingCallback != nullptr)
		pendingCallback->disconnect();
}

Callback::Callback(Callback&& that)
{
	pendingCallback = that.pendingCallback;
	if (pendingCallback)
		pendingCallback->callback = this;
	isValid = that.isValid;
	that.isValid = false;
}
 
void Callback::execute()
{
	if (isValid)
	{
		isValid = false;
		if (pendingCallback != nullptr)
			pendingCallback->disconnect();
		function();
	}
}

std::unique_ptr<PendingCallback> Callback::createPendingCallback()
{
	if (pendingCallback)
		THROW_FATAL_ERROR("Creating pending callback when one already exists");
	std::unique_ptr<PendingCallback> result = std::make_unique<PendingCallback>();
	result->callback = this;
	pendingCallback = result.get();
	return result;
}



//-------------------------------------------------------------------------------------------------------------------

PendingCallback::PendingCallback()
{
}

PendingCallback::~PendingCallback()
{
	std::cout << "~PendingCallback\n";
	cancel();
}

PendingCallback::PendingCallback(PendingCallback&& that)
{
	callback = that.callback;
	if (callback)
		callback->pendingCallback = this;
	that.callback = nullptr;
}

void PendingCallback::disconnect()
{
	std::cout << "PendingCallback::disconnect\n";
	if (callback != nullptr)
	{
		callback->pendingCallback = nullptr;
		callback = nullptr;
	}
}

void PendingCallback::cancel()
{
	std::cout << "PendingCallback::cancel\n";
	if (callback != nullptr)
	{
		callback->isValid = false;
		callback->pendingCallback = nullptr;
		callback = nullptr;
	}
}

DeathNotice::DeathNotice()
{
}

DeathNotice::~DeathNotice()
{
	std::cout << "~DeathNotice" + std::to_string(id) + "\n";
	//printf("DeathNotice destructor");
	//std::cout<<"\nDeathNotice destructor\n";
	execute();
}

DeathNotice::DeathNotice(DeathNotice&& that) : Callback::Callback(std::move(that)){}

std::unique_ptr<PendingCallback> DeathNotice::subscribe(std::function<void()> function)
{
	this->function = function;
	isValid = true;
	return createPendingCallback();
}
