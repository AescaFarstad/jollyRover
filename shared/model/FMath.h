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
	float distanceToLine(const Point &from, const Point &l1, const Point &l2);

	float lerp(float x1, float y1, float x2, float y2, float argument);
	Point lerp(float x1, Point& p1, float x2, Point& p2, float argument);
	///Does not overflow
	uint8_t lerp(float x1, uint8_t y1, float x2, uint8_t y2, float argument);
	float atan2(float y, float x);
	float angleDelta(float from, float to);
	float normalizeAngle(float angle);
	float cos(float angle);
	float sin(float angle);
	//quick simple hash sdbm
	int32_t q_sdbm(int32_t number);
}