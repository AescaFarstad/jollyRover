#pragma once
#include <InputImpulse.h>
#include <InputMessage.h>

class InputImpulseMessage :
	public InputMessage
{
public:
	InputImpulseMessage();

	INPUT_IMPULSE impulse;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};