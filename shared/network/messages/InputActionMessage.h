#pragma once
#include <InputMessage.h>
class InputActionMessage :
	public InputMessage
{
public:
	InputActionMessage();
	~InputActionMessage();

	float angle = 0;
	std::vector<int8_t> downedButtons;
	std::vector<int8_t> uppedButtons;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();
};

