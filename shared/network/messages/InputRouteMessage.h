#pragma once
#include <Point.h>
#include <InputMessage.h>



class InputRouteMessage : 
	public InputMessage
{
public:
	InputRouteMessage();

	std::vector<Point> route;

	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};