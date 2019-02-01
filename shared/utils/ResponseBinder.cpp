#include <ResponseBinder.h>



ResponseBinder::ResponseBinder()
{
}


ResponseBinder::~ResponseBinder()
{
}

void ResponseBinder::bind(ResponseBinding* binding)
{
	BindingStruct couple = {
		binding,
		binding->deathNotice.subscribe([this, binding]() {this->unbind(binding); })
	};
	bindings.push_back(std::move(couple));
}

void ResponseBinder::unbind(ResponseBinding* binding)
{
	for (auto i = bindings.begin(); i != bindings.end(); i++) {
		if (i->binding == binding)
		{
			i->pendingCallback->disconnect();
			bindings.erase(i);
			return;
		}
	}
}

bool ResponseBinder::process(std::unique_ptr<NetworkMessage> msg)
{
	NetworkMessage* p_msg = msg.get();
	for (auto i = bindings.begin(); i != bindings.end(); i++) {
		if (match(i->binding, p_msg))
		{
			i->binding->handle(std::move(msg));

			if (i->binding->callOnce)
			{
				i->pendingCallback->disconnect();
				bindings.erase(i, i + 1);
			}
			return true;
		}
	}
	return false;
}

bool ResponseBinder::match(ResponseBinding* binding, NetworkMessage* msg)
{
	return (!binding->bindsType || binding->typeId == msg->typeId) &&
			(!binding->bindsLogin || binding->login == msg->login) &&
			(!binding->bindsResponseTo || binding->inResponseTo == msg->inResponseTo);
}

GenericRequestBinder::GenericRequestBinder()
{
}

GenericRequestBinder::~GenericRequestBinder()
{
}

bool GenericRequestBinder::match(ResponseBinding* binding, NetworkMessage* msg)
{
	GenericRequestMessage* grm = static_cast<GenericRequestMessage*>(msg);
	return binding->typeId == grm->request;
}
