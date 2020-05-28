#pragma once
#include <NetworkMessage.h>

class DemoDataMessage :
	public NetworkMessage
{
public:
	DemoDataMessage();
	
	std::vector<char> data;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};