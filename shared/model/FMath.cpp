#include "FMath.h"

Point FMath::getEdgeIntersection(Edge &e1, Edge &e2)
{
	Point nullResult;
	nullResult.setToNaN();

	if (std::min(e1.p1->x, e1.p2->x) > std::max(e2.p1->x, e2.p2->x) ||
		std::min(e1.p1->y, e1.p2->y) > std::max(e2.p1->y, e2.p2->y) ||
		std::min(e2.p1->x, e2.p2->x) > std::max(e1.p1->x, e1.p2->x) ||
		std::min(e2.p1->y, e2.p2->y) > std::max(e1.p1->y, e1.p2->y)
		)
		return nullResult;

	Point intersection = getLinesIntersection(*e1.p1, *e1.p2, *e2.p1, *e2.p2);
	if (std::isnan(intersection.x))
		return nullResult;
	
	bool belongs = 
		std::min(e1.p1->x, e1.p2->x) - intersection.x < EPSILON && std::max(e1.p1->x, e1.p2->x) - intersection.x > -EPSILON &&
		std::min(e1.p1->y, e1.p2->y) - intersection.y < EPSILON && std::max(e1.p1->y, e1.p2->y) - intersection.y > -EPSILON &&
		std::min(e2.p1->x, e2.p2->x) - intersection.x < EPSILON && std::max(e2.p1->x, e2.p2->x) - intersection.x > -EPSILON &&
		std::min(e2.p1->y, e2.p2->y) - intersection.y < EPSILON && std::max(e2.p1->y, e2.p2->y) - intersection.y > -EPSILON;
	return belongs ? intersection : nullResult;
}

Point FMath::getLinesIntersection(Point &s11, Point &s12, Point &s21, Point &s22)
{
	float A1 = s12.y - s11.y;
	float B1 = s11.x - s12.x;
	float C1 = A1 * s11.x + B1 * s11.y;
	
	float A2 = s22.y - s21.y;
	float B2 = s21.x - s22.x;
	float C2 = A2 * s21.x + B2 * s21.y;

	float det = A1 * B2 - A2 * B1;
	if (det == 0)
	{
		Point nullResult;
		nullResult.setToNaN();
		return nullResult;
	}
	float intersectionX = (B2 * C1 - B1 * C2) / det;
	float intersectionY = (A1 * C2 - A2 * C1) / det;
	return Point(intersectionX, intersectionY);
}

float FMath::atan2(float y, float x)
{
	float coeff_1 = (float)M_PI / 4;
	float coeff_2 = 3 * coeff_1;
	float abs_y = y > 0 ? y : -y;
	float angle;

	if (abs_y == 0)
		return x >= 0 ? 0 : -(float)M_PI;

	if (x >= 0)
		angle = coeff_1 - coeff_1 * (x - abs_y) / (x + abs_y);
	else
		angle = coeff_2 - coeff_1 * (x + abs_y) / (abs_y - x);

	if (y < 0)
		return(-angle);
	else
		return(angle);
}

float FMath::angleDelta(float from, float to)
{
	from = normalizeAngle(from);
	to = normalizeAngle(to);
	if (from < to)
		return to - from < (float)M_PI ? to - from : to - from - (float)M_PI * 2;
	else
		return from - to < (float)M_PI ? to - from : to - from + (float)M_PI * 2;
}

float FMath::normalizeAngle(float angle)
{
	angle = fmodf(angle, (float)M_PI * 2);
	angle = angle < 0 ? (float)M_PI * 2 + angle : angle;
	return angle;
}
