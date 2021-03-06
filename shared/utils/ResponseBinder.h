#pragma once
#include <Global.h>
#include <NetworkMessage.h>
#include <ResponseBinding.h>
#include <memory>

struct BindingStruct
{	
	BindingStruct(const BindingStruct& that) = delete;
	BindingStruct(BindingStruct&& that) = default;
	BindingStruct& operator=(BindingStruct&& that) = default;
	
	int32_t id;
	std::unique_ptr<ResponseBinding> binding;
	std::unique_ptr<PendingCallback> pendingCallback;
};

class ResponseBinder
{
public:
	int32_t bind(std::unique_ptr<ResponseBinding> binding);
	void unbind(int32_t bindingId);

	bool process(std::unique_ptr<NetworkMessage> msg);
	void traceBindings(std::string reason="noreason");
protected:
	std::vector<BindingStruct> bindings;
	virtual bool match(ResponseBinding* binding, NetworkMessage* msg);
};

class GenericRequestBinder : public ResponseBinder
{
private:
	bool match(ResponseBinding* binding, NetworkMessage* msg) override;
};

