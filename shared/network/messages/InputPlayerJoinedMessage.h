#pragma once
#include <InputMessage.h>
class InputPlayerJoinedMessage :
	public InputMessage
{
public:
	InputPlayerJoinedMessage();

	int32_t login;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};

