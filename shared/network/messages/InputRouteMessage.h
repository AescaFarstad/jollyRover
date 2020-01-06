#pragma once
#include <Point.h>
#include <InputMessage.h>
#include <MessageTypes.h>
#include <SerializationStream.h>


class InputRouteMessage : public InputMessage
{
public:
	InputRouteMessage();
	InputRouteMessage(const std::vector<Point> &route);
	~InputRouteMessage();

	std::vector<Point> route;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;

private:

};