#pragma once
#include <InputMessage.h>
class InputTimeMessage :
	public InputMessage
{
public:
	InputTimeMessage();

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
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};