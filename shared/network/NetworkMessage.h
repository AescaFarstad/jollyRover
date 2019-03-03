#pragma once
#include <cstdint>
#include <memory>
#include <Serializer.h>
#include <ISerializable.h>
#include <MessageTypes.h>

class NetworkMessage :
	public ISerializable
{
public:
	NetworkMessage();
	~NetworkMessage();
	int16_t typeId;
	int32_t initiator_id;
	int32_t login;
	uint32_t stamp;
	int32_t inResponseTo;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();

	static int32_t getMessageId();
private: 
	static int32_t idCounter;
};
