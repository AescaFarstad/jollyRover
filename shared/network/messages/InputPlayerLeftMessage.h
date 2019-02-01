#pragma once
#include <InputMessage.h>
class InputPlayerLeftMessage :
	public InputMessage
{
public:
	InputPlayerLeftMessage();
	~InputPlayerLeftMessage();

	int32_t login;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

