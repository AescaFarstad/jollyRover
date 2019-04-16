#include <FMath.h>


float FMath::lerp(float x1, float y1, float x2, float y2, float argument)
{
	if (x1 == x2)
		return y1 == y2 ? y1 : std::numeric_limits<float>::quiet_NaN();
	return (argument  - x1) * (y1 - y2) / (x1 - x2) + y1;
}


Point FMath::lerp(float x1, Point& p1, float x2, Point& p2, float argument)
{
	if (x1 == x2)
		return p1 == p2 ? p1 : Point::getNullPoint();
	return Point(lerp(x1, p1.x, x2, p2.x, argument), lerp(x1, p1.y, x2, p2.y, argument));
}

uint8_t FMath::lerp(float x1, uint8_t y1, float x2, uint8_t y2, float argument)
{
	if (x1 == x2)
		return y1 == y2 ? y1 : 0;
	float rawValue = lerp(x1, (float)y1, x2, (float)y2, argument);
	if (rawValue < 0)
		rawValue = 0;
	if (rawValue > std::numeric_limits<uint8_t>::max())
		rawValue = std::numeric_limits<uint8_t>::max();
	return (uint8_t)rawValue;
}
	
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

const int SIN_VALUES_LENGTH = 21;
const float SIN_VALUES[SIN_VALUES_LENGTH] = {
			0,
			0.0784590957,
			0.1564344650,
			0.2334453638,
			0.3090169943,
			0.3826834323,
			0.4539904997,
			0.5224985647,
			0.5877852522,
			0.6494480483,
			0.7071067811,
			0.7604059656,
			0.8090169943,
			0.8526401643,
			0.8910065241,
			0.9238795325,
			0.9510565162,
			0.9723699203,
			0.9876883405,
			0.9969173337,
			1
		};


float firstQuarterSin(float angle)
{
	float index = angle / (float)M_PI * 2 * (SIN_VALUES_LENGTH - 1);
				
	return FMath::lerp(std::floor(index), SIN_VALUES[(int)(std::floor(index))], std::ceil(index), SIN_VALUES[(int)(std::ceil(index))], index);
		
}

float FMath::sin(float angle)
{
	angle = normalizeAngle(angle);
			
	if (angle <= (float)M_PI / 2)
		return firstQuarterSin(angle);
	else if (angle <= (float)M_PI)
		return firstQuarterSin((float)M_PI - angle);
	else if (angle <= (float)M_PI * 3 / 2)
		return -firstQuarterSin(angle - (float)M_PI);
	else
		return -firstQuarterSin((float)M_PI * 2 - angle);
}

float FMath::cos(float angle)
{
	return sin((float)M_PI / 2 - angle);
}

int32_t FMath::q_sdbm(int32_t number)
{
	int32_t out = 0;
    int c;
	unsigned char *str = (unsigned char *)&number;
	for(size_t i = 0; i < sizeof(int32_t); i++)
	{
		c = *str++;
		out = c + (out << 6) + (out << 16) - out;
	}
	return out;
}