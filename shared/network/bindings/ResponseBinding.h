#pragma once

#include <Global.h>
#include <NetworkMessage.h>
#include <Callback.h>
#include <functional>
#include <memory>

class ResponseBinding
{
public:
	ResponseBinding();
	~ResponseBinding();
	ResponseBinding(const ResponseBinding& that) = delete;
	ResponseBinding(ResponseBinding&& that) = default;

	int16_t typeId;
	bool bindsType;

	int32_t login;
	bool bindsLogin;

	int32_t inResponseTo;
	bool bindsResponseTo;

	bool callOnce;

	DeathNotice deathNotice;

	virtual void handle(std::unique_ptr<NetworkMessage> message) = 0;
};

