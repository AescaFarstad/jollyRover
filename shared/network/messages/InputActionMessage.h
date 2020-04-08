#pragma once
#include <InputMessage.h>
class InputActionMessage :
	public InputMessage
{
public:
	InputActionMessage();

	float angle = 0;
	std::vector<int16_t> downedButtons;
	std::vector<int16_t> uppedButtons;

	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};