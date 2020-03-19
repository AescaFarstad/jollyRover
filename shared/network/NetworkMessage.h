#pragma once
#include <memory>
#include <MessageTypes.h>

#include <BinarySerializer.h>

class NetworkMessage
{
public:
	NetworkMessage();
	virtual ~NetworkMessage() = default;
	MESSAGE_TYPE typeId;
	int32_t initiator_id;
	int32_t login;
	uint32_t stamp;
	int32_t inResponseTo;

	virtual std::string getName() const;	
	
	virtual void serialize(BinarySerializer& serializer) const;
	virtual void deserialize(BinarySerializer& serializer);

	static int32_t getMessageId();
private: 
	static int32_t idCounter;
};



