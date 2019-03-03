#include <ResponseBinder.h>



ResponseBinder::ResponseBinder()
{
}


ResponseBinder::~ResponseBinder()
{
}

int32_t ResponseBinder::bind(std::unique_ptr<ResponseBinding> binding)
{
	//std::cout << "add binding " << binding->name << "\n";
	int32_t id = S::getId();
	std::unique_ptr<PendingCallback> pendingCallback = binding->deathNotice.subscribe([this, id]() {this->unbind(id); });
	bindings.push_back(BindingStruct {
			id,
			std::move(binding),
			std::move(pendingCallback)
		}
	);
	//traceBindings();
	return id;
}

void ResponseBinder::unbind(int32_t id)
{
	for (auto i = bindings.begin(); i != bindings.end(); i++) {
		if (i->id == id)
		{
			//std::cout<< "unbind " + i->binding->name + "\n";
			auto p = i->pendingCallback.get();
			if (p)
				p->disconnect();
			bindings.erase(i);
			return;
		}
	}
}

bool ResponseBinder::process(std::unique_ptr<NetworkMessage> msg)
{
	NetworkMessage* p_msg = msg.get();
	for (auto i = bindings.begin(); i != bindings.end(); i++) {
		if (match(i->binding.get(), p_msg))
		{
			//std::cout << "apply binding " << i->binding->name << " " <<i->binding->callOnce << "\n";
			i->binding->handle(std::move(msg));

			if (i->binding->callOnce)
			{
				//traceBindings("remove " + i->binding->name + " because callOnce");
				//std::cout<< "unbind because call once " + i->binding->name + "\n";
				i->pendingCallback->disconnect();
				bindings.erase(i);
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

void ResponseBinder::traceBindings(std::string reason)
{
	std::string result = "\n\n" + std::to_string((long long unsigned int)&(*this)) + " bindings " + std::to_string(bindings.size()) + " " + reason + " : \n";
	std::for_each(bindings.begin(), bindings.end(), [&result](BindingStruct &iter){
		result += iter.binding->toString() + "\n has pendingCallback: " + std::to_string((int64_t)iter.pendingCallback.get()) +  "\n";
	});
	S::log.add(result, {LOG_TAGS::UNIQUE});
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