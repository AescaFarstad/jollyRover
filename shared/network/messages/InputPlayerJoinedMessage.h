#pragma once
#include <InputMessage.h>
class InputPlayerJoinedMessage :
	public InputMessage
{
public:
	InputPlayerJoinedMessage();
	~InputPlayerJoinedMessage();

	int32_t login;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

