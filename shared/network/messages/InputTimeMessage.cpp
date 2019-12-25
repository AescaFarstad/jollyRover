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

void InputTimeMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(allowSteps, stream);
	Serializer::read(modifyAllowSteps, stream);
	Serializer::read(timeScale, stream);
	Serializer::read(modifyTimeScale, stream);
	Serializer::read(forcedTimeScale, stream);
	Serializer::read(modifyForcedTimeScale, stream);
	Serializer::read(stepsAtOnce, stream);
	Serializer::read(modifyStepsAtOnce, stream);
	Serializer::read(forcedStepsAtOnce, stream);
	Serializer::read(modifyForcedStepsAtOnce, stream);
}

void InputTimeMessage::serialize(SerializationStream& stream) const
{
	InputMessage::serialize(stream);
	Serializer::write(allowSteps, stream);
	Serializer::write(modifyAllowSteps, stream);
	Serializer::write(timeScale, stream);
	Serializer::write(modifyTimeScale, stream);
	Serializer::write(forcedTimeScale, stream);
	Serializer::write(modifyForcedTimeScale, stream);
	Serializer::write(stepsAtOnce, stream);
	Serializer::write(modifyStepsAtOnce, stream);
	Serializer::write(forcedStepsAtOnce, stream);
	Serializer::write(modifyForcedStepsAtOnce, stream);
}

std::string InputTimeMessage::getName()
{
	return "I-Time";
}