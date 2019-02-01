#pragma once
#include <Point.h>
#include <Edge.h>

class Point;
class Edge;
namespace FMath
{
	const float EPSILON = 0.001f;

	Point getEdgeIntersection(Edge &e1, Edge &e2);
	Point getLinesIntersection(Point &s11, Point &s12, Point &s21, Point &s22);

	float atan2(float y, float x);
	float angleDelta(float from, float to);
	float normalizeAngle(float angle);
}