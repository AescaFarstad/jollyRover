#pragma once
#include <NetworkMessage.h>

class GreetingMessage :
	public NetworkMessage
{
public:
	GreetingMessage();
	~GreetingMessage();

	int32_t login;
	std::vector<int8_t> password;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

