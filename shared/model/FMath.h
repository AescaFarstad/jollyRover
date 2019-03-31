#pragma once
#include <Point.h>
#include <Edge.h>
#include <math.h>
#include <limits>

class Point;
class Edge;
namespace FMath
{
	const float EPSILON = 0.001f;
	const float F_MAX = std::numeric_limits<float>::max();
	const float F_MIN = std::numeric_limits<float>::min();

	Point getEdgeIntersection(Edge &e1, Edge &e2);
	Point getLinesIntersection(Point &s11, Point &s12, Point &s21, Point &s22);

	float lerp(float x1, float y1, float x2, float y2, float argument);
	float atan2(float y, float x);
	float angleDelta(float from, float to);
	float normalizeAngle(float angle);
	float cos(float angle);
	float sin(float angle);
}