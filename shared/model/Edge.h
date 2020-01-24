#pragma once
#include <Point.h>

class Point;
class Edge
{
public:
	Edge() = default;
	Edge(Point* p1, Point* p2);

	Point* p1;
	Point* p2;

	std::string toString();

private:

};