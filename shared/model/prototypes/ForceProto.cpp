#include<ForceProto.h>

void from_json(const nlohmann::json& j, ForceProto& force)
{
	auto spawn = j.at("spawn");
	force.id = j.at("id");
	force.spawnAA.x = spawn.at("x1");
	force.spawnAA.y = spawn.at("y1");
	force.spawnBB.x = spawn.at("x2");
	force.spawnBB.y = spawn.at("y2");
	force.freq[0] = j.at("freq")[0];
	force.freq[1] = j.at("freq")[1];
	force.alwaysOn = j.at("alwaysOn");
}