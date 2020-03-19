#pragma once
#include <InputMessage.h>

class InputPlayerJoinedMessage :
	public InputMessage
{
public:
	InputPlayerJoinedMessage();

	int32_t login;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};