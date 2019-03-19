#pragma once
#include <ResponseBinding.h>

template <typename T>
class AnonymousBinding :
	public ResponseBinding
{
	using PNetworkMessage = std::unique_ptr<NetworkMessage>;

public:
	AnonymousBinding(const AnonymousBinding& that) = delete;
	AnonymousBinding(AnonymousBinding&& that) = default;

	AnonymousBinding(std::string name, T handler)
		: ResponseBinding(std::move(name))
		, m_handler(std::move(handler))
	{
	}

	AnonymousBinding* bindByType(int16_t typeId)
	{
		bindsType = true;
		this->typeId = typeId;
		return this;
	}

	AnonymousBinding* bindByLogin(int32_t login)
	{
		bindsLogin = true;
		this->login = login;
		return this;
	}

	AnonymousBinding* bindByResponseTo(int32_t inResponseTo)
	{
		bindsResponseTo = true;
		this->inResponseTo = inResponseTo;
		return this;
	}

	AnonymousBinding* setCallOnce(bool callOnce)
	{
		this->callOnce = callOnce;
		return this;
	}

	void handle(PNetworkMessage message) override
	{
		m_handler(std::move(message));
	}

private:
	T m_handler;
};

template <typename T>
std::unique_ptr<AnonymousBinding<T>> makeAnonymousBinding(std::string name, T&& handler)
{
	return std::make_unique<AnonymousBinding<T>>(std::move(name), std::forward<T>(handler));
}
