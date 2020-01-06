#pragma once
#include <InputMessage.h>
#include <DebugAction.h>
#include <Point.h>


class InputDebugMessage :
	public InputMessage
{
public:
	InputDebugMessage();

	DEBUG_ACTION action;
	Point coords;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};