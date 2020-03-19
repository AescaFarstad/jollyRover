#pragma once
#include <InputMessage.h>

class InputPlayerLeftMessage :
	public InputMessage
{
public:
	InputPlayerLeftMessage();

	int32_t login;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};