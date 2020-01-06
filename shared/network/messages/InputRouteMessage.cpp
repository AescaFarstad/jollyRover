#include <InputRouteMessage.h>

InputRouteMessage::InputRouteMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG;
}

inline void InputRouteMessage::deserialize(SerializationStream &stream)
{
	InputMessage::deserialize(stream);
	Serializer::readVector(route, stream);
}

inline void InputRouteMessage::serialize(SerializationStream &stream) const
{
	InputMessage::serialize(stream);
	Serializer::writeVector(route, stream); 
}

inline std::string InputRouteMessage::getName() const
{
	return "I-rout";
}
