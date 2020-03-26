#pragma once
#include <NetworkMessage.h>

class StateRequestMessage :
	public NetworkMessage
{
public:
	StateRequestMessage();

	std::vector<uint32_t> states;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};