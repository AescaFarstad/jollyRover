#pragma once
#include <NetworkMessage.h>
#include <vector>

class GreetingMessage :
	public NetworkMessage
{
public:
	GreetingMessage();

	int32_t login;
	std::vector<int8_t> password;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};