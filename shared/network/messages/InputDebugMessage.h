#pragma once
#include <DebugAction.h>
#include <Point.h>
#include <InputMessage.h>


class InputDebugMessage :
	public InputMessage
{
public:
	InputDebugMessage();

	DEBUG_ACTION action;
	Point coords;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};