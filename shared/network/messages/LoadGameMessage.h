#pragma once
#include <InputMessage.h>
class LoadGameMessage :
	public InputMessage
{
public:
	LoadGameMessage();

	std::vector<char> state;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};