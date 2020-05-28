#pragma once
#include <NetworkMessage.h>

class DemoListMessage :
	public NetworkMessage
{
public:
	DemoListMessage();

	std::vector<std::string> list;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};