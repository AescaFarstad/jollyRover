#pragma once
#include <NetworkMessage.h>

class DemoRequestMessage :
	public NetworkMessage
{
public:
	DemoRequestMessage();

	std::string demoName;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};