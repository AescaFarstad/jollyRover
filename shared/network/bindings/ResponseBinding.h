#pragma once

#include <Global.h>
#include <NetworkMessage.h>
#include <Callback.h>
#include <functional>
#include <memory>

class ResponseBinding
{
public:
	ResponseBinding(std::string name);
	virtual ~ResponseBinding();
	ResponseBinding(const ResponseBinding& that) = delete;
	ResponseBinding(ResponseBinding&& that) = default;
	
	std::string name;

	int16_t typeId;
	bool bindsType;

	int32_t login;
	bool bindsLogin;

	int32_t inResponseTo;
	bool bindsResponseTo;

	bool callOnce;

	DeathNotice deathNotice;
	
	std::string moved;

	virtual void handle(std::unique_ptr<NetworkMessage> message) = 0;	
	
	std::string toString();
};

