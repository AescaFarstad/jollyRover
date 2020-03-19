#include <CarState.h>

CarState::CarState()
{
	unit.host_.type = ENTTITY_TYPE::CAR;
	unit.host_.pointer = this;
}

const Point& CarState::getLocation()
{
	return unit.location;
}
