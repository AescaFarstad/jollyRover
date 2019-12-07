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
	car.accel = j.at("accel");
	car.size = j.at("size");
	car.playerCarGunTextureName = j.at("gunTexture").at("player");
	car.playerCarHullTextureName = j.at("hullTexture").at("player");
	car.opponentCarGunTextureName = j.at("gunTexture").at("opponent");
	car.opponentCarHullTextureName = j.at("hullTexture").at("opponent");
}
