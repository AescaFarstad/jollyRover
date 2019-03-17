#pragma once
#include <json.hpp>
#include <Point.h>
#include <Edge.h>
#include <limits>

using json = nlohmann::json;

class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	Obstacle(Obstacle const &other);

	std::vector<Point> vertices;
	std::vector<Edge> edges;
	Point AA;
	Point BB;
	Point centroid;

	bool isInside(Point &p);

private:

};

void from_json(const json &j, Obstacle &obstacle);