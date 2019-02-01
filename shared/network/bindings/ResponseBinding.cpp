#include <ResponseBinding.h>



ResponseBinding::ResponseBinding()
{
	bindsLogin = false;
	bindsResponseTo = false;
	bindsType = false;
	callOnce = true;
}


ResponseBinding::~ResponseBinding()
{
}
/*
ResponseBinding::ResponseBinding(ResponseBinding&& that)
{
	
}*/

