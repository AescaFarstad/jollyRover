#pragma once
#include <NetworkMessage.h>

class GenericRequestMessage :
	public NetworkMessage
{
public:
	GenericRequestMessage();

	REQUEST_TYPE request;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};

