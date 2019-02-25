#pragma once
#include <ResponseBinding.h>

class AnonymousBinding :
	public ResponseBinding
{
public:
	AnonymousBinding(std::string name);
	virtual ~AnonymousBinding();
	AnonymousBinding(const AnonymousBinding& that) = delete;
	AnonymousBinding(AnonymousBinding&& that);

	AnonymousBinding* bindByType(int16_t typeId);
	AnonymousBinding* bindByLogin(int32_t login);
	AnonymousBinding* bindByResponseTo(int32_t inResponseTo);
	AnonymousBinding* setCallOnce(bool callOnce);
	AnonymousBinding* setHandler(std::unique_ptr<std::function<void(std::unique_ptr<NetworkMessage>)>> handler);


	void handle(std::unique_ptr<NetworkMessage> message) override;

private:
	std::unique_ptr<std::function<void(std::unique_ptr<NetworkMessage>)>> handler;
};

