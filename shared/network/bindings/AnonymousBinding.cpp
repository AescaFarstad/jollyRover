#include <AnonymousBinding.h>



AnonymousBinding::AnonymousBinding()
{
}


AnonymousBinding::~AnonymousBinding()
{
}


AnonymousBinding* AnonymousBinding::bindByType(int16_t typeId)
{
	bindsType = true;
	this->typeId = typeId;
	return this;
}

AnonymousBinding* AnonymousBinding::bindByLogin(int32_t login)
{
	bindsLogin = true;
	this->login = login;
	return this;
}

AnonymousBinding* AnonymousBinding::bindByResponseTo(int32_t inResponseTo)
{
	bindsResponseTo = true;
	this->inResponseTo = inResponseTo;
	return this;
}

AnonymousBinding* AnonymousBinding::setCallOnce(bool callOnce)
{
	this->callOnce = callOnce;
	return this;
}

AnonymousBinding* AnonymousBinding::setHandler(std::function<void(std::unique_ptr<NetworkMessage>)>* handler)
{
	this->handler = handler;
	return this;
}

void AnonymousBinding::handle(std::unique_ptr<NetworkMessage> message)
{
	(*handler)(std::move(message));
}