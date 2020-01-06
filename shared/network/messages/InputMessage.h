#pragma once
#include <NetworkMessage.h>
#include <vector>


class InputMessage :
	public NetworkMessage
{
public:
	InputMessage() = default;
	
	int32_t serverStamp;
	int16_t localId;
	int16_t serverId;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};

