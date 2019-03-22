#include <InputTimeMessage.h>

InputTimeMessage::InputTimeMessage()
{
	typeId = MessageTypes::TYPE_INPUT_TIME_MSG;
	
	modifyAllowSteps = false;
	modifyForcedTimeScale = false;
	modifyTimeScale = false;
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
}

void InputTimeMessage::serialize(SerializationStream& stream)
{
	InputMessage::serialize(stream);
	Serializer::write(allowSteps, stream);
	Serializer::write(modifyAllowSteps, stream);
	Serializer::write(timeScale, stream);
	Serializer::write(modifyTimeScale, stream);
	Serializer::write(forcedTimeScale, stream);
	Serializer::write(modifyForcedTimeScale, stream);
}

std::string InputTimeMessage::getName()
{
	return "I-Time";
}