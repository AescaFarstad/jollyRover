#pragma once
#include <InputMessage.h>
class InputPlayerLeftMessage :
	public InputMessage
{
public:
	InputPlayerLeftMessage();

	int32_t login;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};

