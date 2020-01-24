#pragma once
#include <json.hpp>

class TextureDef;
class CarProto
{
public:
	int16_t id;
	int32_t maxHealth;
	int32_t size;
	std::string playerCarHullTextureName;
	std::string playerCarGunTextureName;
	std::string opponentCarHullTextureName;
	std::string opponentCarGunTextureName;
	TextureDef* playerCarHullTexture;
	TextureDef* playerCarGunTexture;
	TextureDef* opponentCarHullTexture;
	TextureDef* opponentCarGunTexture;
	float speed;
	float accel;

private:

};

void from_json(const nlohmann::json& j, CarProto& car);