#pragma once
#include <NetworkMessage.h>

class GenericRequestMessage :
	public NetworkMessage
{
public:
	GenericRequestMessage();
	~GenericRequestMessage();

	RequestTypes request;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

