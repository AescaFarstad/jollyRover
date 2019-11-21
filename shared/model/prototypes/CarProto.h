#pragma once
#include <json.hpp>
using json = nlohmann::json;

class TextureDef;
class CarProto
{
public:
	CarProto();
	~CarProto();

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

private:

};

void from_json(const json &j, CarProto &car);