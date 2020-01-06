#include <InputRouteMessage.h>

InputRouteMessage::InputRouteMessage()
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG;
}

InputRouteMessage::InputRouteMessage(const std::vector<Point> &route)
{
	typeId = MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG;
	this->route = route;
}

InputRouteMessage::~InputRouteMessage()
{
}

inline void InputRouteMessage::deserialize(SerializationStream &stream)
{
	InputMessage::deserialize(stream);
	Serializer::readVector(route, stream);
}

inline void InputRouteMessage::serialize(SerializationStream &stream) const
{
	//SerializationStream::AGA = true;
	InputMessage::serialize(stream);
	//SerializationStream::AGA = false;
	//std::string h;
	//h.assign(stream.readAllAsHex(), stream.getLength() * 3);
	//S::log.add("Inter ser result: " + h);
	Serializer::writeVector(route, stream); 
	//h.assign(stream.readAllAsHex(), stream.getLength() * 3);
	//S::log.add("post ser result: " + h);
}

inline std::string InputRouteMessage::getName() const
{
	return "I-rout";
}
