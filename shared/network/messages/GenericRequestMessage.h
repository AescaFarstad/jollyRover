#pragma once
#include <NetworkMessage.h>

class GenericRequestMessage :
	public NetworkMessage
{
public:
	GenericRequestMessage();
	~GenericRequestMessage();

	RequestTypes request;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() override;
};

