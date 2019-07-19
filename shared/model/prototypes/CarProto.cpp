#include <CarProto.h>

CarProto::CarProto()
{
}

CarProto::~CarProto()
{
}

void from_json(const json &j, CarProto &car)
{
	car.id = j.at("id");
	car.maxHealth = j.at("maxHealth");
	car.speed = j.at("speed");
	car.size = j.at("size");
}
