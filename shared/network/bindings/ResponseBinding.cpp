#include <ResponseBinding.h>



ResponseBinding::ResponseBinding(std::string name)
{
	bindsLogin = false;
	bindsResponseTo = false;
	bindsType = false;
	callOnce = true;
	this->name = name;
}


ResponseBinding::~ResponseBinding()
{	
	std::cout << "~ResponseBinding\n";
}
/*
ResponseBinding::ResponseBinding(ResponseBinding&& that)
{
	this->name = std::move(that.name);
	this->typeId = that.typeId;
	this->bindsType = that.bindsType;
	this->login = that.login;
	this->bindsLogin = that.bindsLogin;
	this->inResponseTo = that.inResponseTo;
	this->bindsResponseTo = that.bindsResponseTo;
	this->callOnce = that.callOnce;
	this->deathNotice = std::move(that.deathNotice);
}
*/

std::string  ResponseBinding::toString()
{
	std::string result = name + " ";
	if (bindsType)
		result += "t:" + std::to_string(typeId) + " ";
	if (bindsLogin)
		result += "login:" + std::to_string(login) + " ";
	if (bindsResponseTo)
		result += "responseTo:" + std::to_string(inResponseTo) + " ";
	if (callOnce)
		result += "callOnce ";
	result += moved;
	return result;
}
