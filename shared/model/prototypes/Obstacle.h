#pragma once
#include <json.hpp>
#include <Point.h>
#include <Edge.h>
#include <limits>

class Obstacle
{
public:
	Obstacle() = default;
	Obstacle(const Obstacle& other);

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

void from_json(const nlohmann::json& j, Obstacle& obstacle);