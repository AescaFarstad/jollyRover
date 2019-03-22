#pragma once
#include <InputMessage.h>
class InputTimeMessage :
	public InputMessage
{
public:
	InputTimeMessage();
	~InputTimeMessage() = default;

	int32_t allowSteps;
	bool modifyAllowSteps;
	float timeScale;
	bool modifyTimeScale;
	float forcedTimeScale;
	bool modifyForcedTimeScale;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) override;
	std::string getName() override;
};
