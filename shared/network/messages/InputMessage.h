#pragma once
#include <NetworkMessage.h>
#include <vector>


class InputMessage :
	public NetworkMessage
{
public:
	InputMessage();
	~InputMessage();
	int32_t serverStamp;
	int16_t localId;
	int16_t serverId;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

