#pragma once
#include <InputMessage.h>
class InputActionMessage :
	public InputMessage
{
public:
	InputActionMessage();

	float angle = 0;
	std::vector<int8_t> downedButtons;
	std::vector<int8_t> uppedButtons;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};

