#pragma once
#include <Global.h>
#include <NetworkMessage.h>
#include <ResponseBinding.h>
#include <GenericRequestMessage.h>
#include <functional>
#include <memory>

struct BindingStruct
{
	
	BindingStruct(const BindingStruct& that) = delete;
	BindingStruct(BindingStruct&& that) = default;
	BindingStruct& operator=(BindingStruct&& that) = default;
	
	ResponseBinding* binding;
	std::unique_ptr<PendingCallback> pendingCallback;
};

class ResponseBinder
{
public:
	ResponseBinder();
	~ResponseBinder();

	void bind(ResponseBinding* binding);
	void unbind(ResponseBinding* binding);

	bool process(std::unique_ptr<NetworkMessage> msg);
private:
	std::vector<BindingStruct> bindings;
	virtual bool match(ResponseBinding* binding, NetworkMessage* msg);
};

class GenericRequestBinder : public ResponseBinder
{
public:
	GenericRequestBinder();
	~GenericRequestBinder();
private:
	bool match(ResponseBinding* binding, NetworkMessage* msg) override;
};

