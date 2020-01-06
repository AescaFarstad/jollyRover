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
	int32_t stepsAtOnce;
	bool modifyStepsAtOnce;
	int32_t forcedStepsAtOnce;
	bool modifyForcedStepsAtOnce;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;
};
