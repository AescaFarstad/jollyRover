#pragma once
#include <NetworkMessage.h>

class GenericRequestMessage :
	public NetworkMessage
{
public:
	GenericRequestMessage();

	REQUEST_TYPE request;
	
	std::string getName() const override;	
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};