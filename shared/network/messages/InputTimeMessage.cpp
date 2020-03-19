#include <InputTimeMessage.h>

InputTimeMessage::InputTimeMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_TIME_MSG;
	
	modifyAllowSteps = false;
	modifyForcedTimeScale = false;
	modifyTimeScale = false;
	modifyStepsAtOnce = false;
	modifyForcedStepsAtOnce = false;
}

std::string InputTimeMessage::getName() const
{
	return "I-Time";
}

void InputTimeMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, allowSteps);
	WRITE_FIELD((*this), serializer, modifyAllowSteps);
	WRITE_FIELD((*this), serializer, timeScale);
	WRITE_FIELD((*this), serializer, modifyTimeScale);
	WRITE_FIELD((*this), serializer, forcedTimeScale);
	WRITE_FIELD((*this), serializer, modifyForcedTimeScale);
	WRITE_FIELD((*this), serializer, stepsAtOnce);
	WRITE_FIELD((*this), serializer, modifyStepsAtOnce);
	WRITE_FIELD((*this), serializer, forcedStepsAtOnce);
	WRITE_FIELD((*this), serializer, modifyForcedStepsAtOnce);
}

void InputTimeMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, allowSteps);
	READ__FIELD((*this), serializer, modifyAllowSteps);
	READ__FIELD((*this), serializer, timeScale);
	READ__FIELD((*this), serializer, modifyTimeScale);
	READ__FIELD((*this), serializer, forcedTimeScale);
	READ__FIELD((*this), serializer, modifyForcedTimeScale);
	READ__FIELD((*this), serializer, stepsAtOnce);
	READ__FIELD((*this), serializer, modifyStepsAtOnce);
	READ__FIELD((*this), serializer, forcedStepsAtOnce);
	READ__FIELD((*this), serializer, modifyForcedStepsAtOnce);
}