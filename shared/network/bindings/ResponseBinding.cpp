#include <ResponseBinding.h>



ResponseBinding::ResponseBinding(std::string name)
{
	bindsLogin = false;
	bindsResponseTo = false;
	bindsMsgType = false;
	bindsGenericType = false;
	callOnce = true;
	this->name = name;
}


ResponseBinding::~ResponseBinding()
{	
}

std::string  ResponseBinding::toString()
{
	std::string result = name + " ";
	if (bindsMsgType)
		result += "t:" + std::to_string((int16_t)msgTypeId) + " ";
	if (bindsGenericType)
		result += "t:" + std::to_string((int16_t)genericTypeId) + " ";
	if (bindsLogin)
		result += "login:" + std::to_string(login) + " ";
	if (bindsResponseTo)
		result += "responseTo:" + std::to_string(inResponseTo) + " ";
	if (callOnce)
		result += "callOnce ";
	return result;
}
