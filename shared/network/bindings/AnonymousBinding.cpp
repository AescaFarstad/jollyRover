#include <AnonymousBinding.h>



AnonymousBinding::AnonymousBinding(std::string name) : ResponseBinding(name)
{
}


AnonymousBinding::~AnonymousBinding()
{
}


AnonymousBinding::AnonymousBinding(AnonymousBinding&& that) : ResponseBinding::ResponseBinding(std::move(that))
{
	handler = std::move(that.handler);
}


AnonymousBinding* AnonymousBinding::bindByMsgType(MessageTypes typeId)
{
	bindsMsgType = true;
	this->msgTypeId = typeId;
	return this;
}

AnonymousBinding* AnonymousBinding::bindByGenericType(RequestTypes typeId)
{
	bindsGenericType = true;
	this->genericTypeId = typeId;
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

AnonymousBinding* AnonymousBinding::setHandler(std::unique_ptr<std::function<void(std::unique_ptr<NetworkMessage>)>> handler)
{
	this->handler = std::move(handler);
	return this;
}

void AnonymousBinding::handle(std::unique_ptr<NetworkMessage> message)
{
	handler->operator()(std::move(message));
}