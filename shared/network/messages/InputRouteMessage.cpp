#include <InputRouteMessage.h>
#include <MessageTypes.h>

InputRouteMessage::InputRouteMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG;
}

inline std::string InputRouteMessage::getName() const
{
	return "I-rout";
}

void InputRouteMessage::serialize(BinarySerializer& serializer) const
{
	InputMessage::serialize(serializer);
	WRITE_FIELD((*this), serializer, route);
}

void InputRouteMessage::deserialize(BinarySerializer& serializer)
{
	InputMessage::deserialize(serializer);
	READ__FIELD((*this), serializer, route);
}
