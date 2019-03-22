#pragma once
#include <InputMessage.h>
class LoadGameMessage :
	public InputMessage
{
public:
	LoadGameMessage();
	~LoadGameMessage();

	char* state;
	uint32_t stateLength;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) override;
	std::string getName() override;
};
