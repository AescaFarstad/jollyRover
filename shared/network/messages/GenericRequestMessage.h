#pragma once
#include <NetworkMessage.h>

class GenericRequestMessage :
	public NetworkMessage
{
public:
	GenericRequestMessage();
	~GenericRequestMessage();

	int16_t request;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

