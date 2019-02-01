#pragma once
#include <Point.h>
#include <InputMessage.h>
#include <MessageTypes.h>
#include <SerializationStream.h>


class InputRouteMessage : public InputMessage
{
public:
	InputRouteMessage();
	InputRouteMessage(std::vector<Point> &route);
	~InputRouteMessage();

	std::vector<Point> route;

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
	virtual std::string getName();

private:

};