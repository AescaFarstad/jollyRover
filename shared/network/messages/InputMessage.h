#pragma once
#include <NetworkMessage.h>
#include <vector>


class InputMessage :
	public NetworkMessage
{
public:
	InputMessage() = default;
	
	///Execute this input at
	int32_t serverStamp;
	
	int16_t localId;
	int16_t serverId;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};