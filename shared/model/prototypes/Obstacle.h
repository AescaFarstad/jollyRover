#pragma once
#include <json.hpp>
#include <Point.h>
#include <Edge.h>
#include <limits>

using json = nlohmann::json;

class Obstacle
{
public:
	Obstacle() = default;
	Obstacle(Obstacle const& other);

	std::vector<Point> vertices;
	std::vector<Point> extendedVertices[2];
	std::vector<Edge> edges;
	std::vector<Edge> extendedEdges;
	Point AA;
	Point BB;
	Point centroid;

	bool isInside(Point& p);
	
	const Point& getAA();
	const Point& getBB();
	const Point& getCenter();
	bool hitTest(Point& p);
	bool hitTest(Edge& e);

private:

};

void from_json(const json& j, Obstacle& obstacle);