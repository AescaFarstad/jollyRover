#pragma once
#include <InputMessage.h>
class LoadGameMessage :
	public InputMessage
{
public:
	LoadGameMessage();

	SerializationStream state;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};
