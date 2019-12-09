#pragma once
#include <InputMessage.h>
#include <InputImpulse.h>

class InputImpulseMessage :
	public InputMessage
{
public:
	InputImpulseMessage();
	~InputImpulseMessage();

	INPUT_IMPULSE impulse;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() override;
};