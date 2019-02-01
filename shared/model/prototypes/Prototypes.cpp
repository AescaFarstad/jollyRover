#include <Prototypes.h>

Prototypes::Prototypes()
{
}

Prototypes::~Prototypes()
{
}

const CarProto* Prototypes::getCar(int16_t id)
{
	return nullptr;
}

const LootProto* Prototypes::getLoot(int16_t id)
{
	return nullptr;
}

void Prototypes::load(json &source)
{
	variables.load(source.at("variables"));
	obstacles = source.at("obstacles").get<std::vector<Obstacle>>();
	cars = source.at("cars").get<std::vector<CarProto>>();
	/*
	for (auto &ob : obstacles)
	{
		for (auto &ver : ob.vertices)
		{
			S::log.add("post vert " + ver.toString());
			S::log.add("adr: " + std::to_string((uint32_t)&ver));
		}
		for (auto &edg : ob.edges)
		{
			S::log.add("post edge " + edg.p1->toString() + edg.p2->toString());
			S::log.add("adr: " + std::to_string((uint32_t)edg.p1));
		}
	}*/

	//cars = source.at("loot").get<std::vector<LootProto>>();
	//loot = source.at("cars").get<std::vector<CarProto>>();
}
