#pragma once
#include <Point.h>

class Point;
class Edge
{
public:
	Edge();
	Edge(Point* p1, Point* p2);
	~Edge();

	Point* p1;
	Point* p2;

	std::string toString();

private:

};