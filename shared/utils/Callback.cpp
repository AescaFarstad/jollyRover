#include <Callback.h>

Callback::Callback()
{
	m_pendingCallback = nullptr;
	m_isValid = false;
}

Callback::Callback(std::function<void()> function)
{
	this->m_function = function;
	m_isValid = true;
	m_pendingCallback = nullptr;
}

Callback::~Callback()
{
	if (m_pendingCallback != nullptr)
		m_pendingCallback->disconnect();
}

Callback::Callback(Callback&& that)
{
	m_pendingCallback = that.m_pendingCallback;
	if (m_pendingCallback)
		m_pendingCallback->callback = this;
	m_isValid = that.m_isValid;
	that.m_isValid = false;
}

Callback& Callback::operator=(Callback&& that)
{
	if (this != &that)
	{
		m_pendingCallback = that.m_pendingCallback;
		if (m_pendingCallback)
			m_pendingCallback->callback = this;
		m_isValid = that.m_isValid;
		that.m_isValid = false;		
	}
	return *this;
}
 
void Callback::execute()
{
	if (m_isValid)
	{
		m_isValid = false;
		if (m_pendingCallback != nullptr)
			m_pendingCallback->disconnect();
		m_function();
	}
}

std::unique_ptr<PendingCallback> Callback::createPendingCallback()
{
	if (m_pendingCallback)
		THROW_FATAL_ERROR("Creating pending callback when one already exists");
	std::unique_ptr<PendingCallback> result = std::make_unique<PendingCallback>();
	result->callback = this;
	m_pendingCallback = result.get();
	return result;
}



//-------------------------------------------------------------------------------------------------------------------

PendingCallback::PendingCallback()
{
}

PendingCallback::~PendingCallback()
{
	cancel();
}

PendingCallback::PendingCallback(PendingCallback&& that)
{
	callback = that.callback;
	if (callback)
		callback->m_pendingCallback = this;
	that.callback = nullptr;
}


PendingCallback& PendingCallback::operator=(PendingCallback&& that)
{
	if (this != &that)
	{
		callback = that.callback;
		if (callback)
			callback->m_pendingCallback = this;
		that.callback = nullptr;		
	}
	return *this;
}

void PendingCallback::disconnect()
{
	if (callback != nullptr)
	{
		callback->m_pendingCallback = nullptr;
		callback = nullptr;
	}
}

void PendingCallback::cancel()
{
	if (callback != nullptr)
	{
		callback->m_isValid = false;
		callback->m_pendingCallback = nullptr;
		callback = nullptr;
	}
}

DeathNotice::DeathNotice()
{
}

DeathNotice::~DeathNotice()
{
	execute();
}

DeathNotice::DeathNotice(DeathNotice&& that) : Callback::Callback(std::move(that)){}

std::unique_ptr<PendingCallback> DeathNotice::subscribe(std::function<void()> function)
{
	this->m_function = function;
	m_isValid = true;
	return createPendingCallback();
}
