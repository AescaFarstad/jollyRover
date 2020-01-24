#pragma once
#include <Point.h>
#include <InputMessage.h>
#include <SerializationStream.h>


class InputRouteMessage : public InputMessage
{
public:
	InputRouteMessage();

	std::vector<Point> route;

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) const override;
	std::string getName() const override;

private:

};