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

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() override;
};

