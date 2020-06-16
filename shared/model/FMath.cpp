#include <FMath.h>


float FMath::lerp(float x1, float y1, float x2, float y2, float argument)
{
	if (x1 == x2)
		return y1 == y2 ? y1 : std::numeric_limits<float>::quiet_NaN();
	return (argument  - x1) * (y1 - y2) / (x1 - x2) + y1;
}

float FMath::nlerp(float x1, float y1, float x2, float y2, float argument, float power)
{
	if (x1 == x2)
		return y1 == y2 ? y1 : std::numeric_limits<float>::quiet_NaN();
	//(double) - to lower GLIBC version
	x1 = std::pow((double)x1, (double)power);
	x2 = std::pow((double)x2, (double)power);
	argument = std::pow((double)argument, (double)power);
	return (argument  - x1) * (y1 - y2) / (x1 - x2) + y1;
}

float FMath::lerpClipped(float x1, float y1, float x2, float y2, float argument)
{
	float min = std::min(y1, y2);
	float max = std::max(y1, y2);
	
	return std::min(max, std::max(min, lerp(x1, y1, x2, y2, argument)));
}


Point FMath::lerp(float x1, Point& p1, float x2, Point& p2, float argument)
{
	if (x1 == x2)
		return p1 == p2 ? p1 : Point::getNullPoint();
	return Point(lerp(x1, p1.x, x2, p2.x, argument), lerp(x1, p1.y, x2, p2.y, argument));
}

uint8_t FMath::lerp_ui8(float x1, uint8_t y1, float x2, uint8_t y2, float argument)
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
	
Point FMath::getEdgeIntersection(Edge& e1, Edge& e2)
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

Point FMath::getLinesIntersection(Point& s11, Point& s12, Point& s21, Point& s22)
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

float FMath::distanceToLine(const Point& from, const Point& l1, const Point& l2)
{
	if (l1.x == l2.x)
	{
		if (l1.y == l2.y)
			THROW_FATAL_ERROR("Invalid line");
		return l1.y > l2.y ? from.x - l1.x  : l1.x - from.x;
	}
	
	float a = l1.y - l2.y;
	float b = l2.x - l1.x;
	float c = l1.crossProduct(l2);
	
	return (a * from.x + b * from.y + c) / std::sqrt(a * a + b * b);
}


std::array<Point, 2> FMath::intercept(const Point& origin, const Point& target, const Point& velocity, float speed)
{
	Point delta = target - origin;
	float a = velocity.x * velocity.x + velocity.y * velocity.y - speed * speed;
	float b = 2 * (velocity.x * delta.x + velocity.y * delta.y);
	float c = delta.x * delta.x + delta.y * delta.y;
	
	if (std::fabs(b) < FMath::EPSILON)
		return {Point::getNullPoint(), Point::getNullPoint()};
	
	std::array<float, 2> t = {-c/b, -c/b};
	if (std::fabs(a) > FMath::EPSILON)
	{
		float d = b * b - 4 * a * c;
		if (d < 0) 
			return {Point::getNullPoint(), Point::getNullPoint()};
		d = std::sqrt(d);
		a *= 2;
		t[0] = (-b - d) / a;
		t[1] = (-b + d) / a;
	}
	
	std::array<Point, 2> result;
	
	result[0] = t[0] > 0 ? target + t[0] * velocity : Point::getNullPoint();
	result[1] = t[1] > 0 ? target + t[1] * velocity : Point::getNullPoint();
	
	return result;
}

//https://www.dsprelated.com/showarticle/1052.php
// Polynomial approximating arctangenet on the range -1,1.
// Max error < 0.005 (or 0.29 degrees)
float appAtan(float z)
{
    const float n1 = 0.97239411f;
    const float n2 = -0.19194795f;
    return (n1 + n2 * z * z) * z;
}

float FMath::atan2(float y, float x)
{
    if (x != 0.0f)
    {
        if (fabsf(x) > fabsf(y))
        {
            const float z = y / x;
            if (x > 0.0)
            {
                // atan2(y,x) = atan(y/x) if x > 0
                return appAtan(z);
            }
            else if (y >= 0.0)
            {
                // atan2(y,x) = atan(y/x) + PI if x < 0, y >= 0
                return appAtan(z) + M_PI;
            }
            else
            {
                // atan2(y,x) = atan(y/x) - PI if x < 0, y < 0
                return appAtan(z) - M_PI;
            }
        }
        else // Use property atan(y/x) = PI/2 - atan(x/y) if |y/x| > 1.
        {
            const float z = x / y;
            if (y > 0.0)
            {
                // atan2(y,x) = PI/2 - atan(x/y) if |y/x| > 1, y > 0
                return -appAtan(z) + M_PI_2;
            }
            else
            {
                // atan2(y,x) = -PI/2 - atan(x/y) if |y/x| > 1, y < 0
                return -appAtan(z) - M_PI_2;
            }
        }
    }
    else
    {
        if (y > 0.0f) // x = 0, y > 0
        {
            return M_PI_2;
        }
        else if (y < 0.0f) // x = 0, y < 0
        {
            return -M_PI_2;
        }
    }
    return 0.0f; // x,y = 0. Could return NaN instead.
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

const int32_t SIN_VALUES_LENGTH = 101;
const float SIN_VALUES[SIN_VALUES_LENGTH] = {
			0.000000,
			0.015707,
			0.031411,
			0.047106,
			0.062791,
			0.078459,
			0.094108,
			0.109734,
			0.125333,
			0.140901,
			0.156434,
			0.171929,
			0.187381,
			0.202787,
			0.218143,
			0.233445,
			0.248690,
			0.263873,
			0.278991,
			0.294040,
			0.309017,
			0.323917,
			0.338738,
			0.353475,
			0.368125,
			0.382683,
			0.397148,
			0.411514,
			0.425779,
			0.439939,
			0.453990,
			0.467930,
			0.481754,
			0.495459,
			0.509041,
			0.522499,
			0.535827,
			0.549023,
			0.562083,
			0.575005,
			0.587785,
			0.600420,
			0.612907,
			0.625243,
			0.637424,
			0.649448,
			0.661312,
			0.673013,
			0.684547,
			0.695913,
			0.707107,
			0.718126,
			0.728969,
			0.739631,
			0.750111,
			0.760406,
			0.770513,
			0.780430,
			0.790155,
			0.799685,
			0.809017,
			0.818150,
			0.827081,
			0.835807,
			0.844328,
			0.852640,
			0.860742,
			0.868632,
			0.876307,
			0.883766,
			0.891007,
			0.898028,
			0.904827,
			0.911403,
			0.917755,
			0.923880,
			0.929776,
			0.935444,
			0.940881,
			0.946085,
			0.951057,
			0.955793,
			0.960294,
			0.964557,
			0.968583,
			0.972370,
			0.975917,
			0.979223,
			0.982287,
			0.985109,
			0.987688,
			0.990024,
			0.992115,
			0.993961,
			0.995562,
			0.996917,
			0.998027,
			0.998890,
			0.999507,
			0.999877,
			1.000000
		};


float firstQuarterSin(float angle)
{
	float index = angle / (float)M_PI * 2 * (SIN_VALUES_LENGTH - 1);
				
	return FMath::lerp(
						std::floor(index), 
						SIN_VALUES[(int)(std::floor(index))], 
						std::ceil(index), 
						SIN_VALUES[(int)(std::ceil(index))], 
						index
					);
		
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
    int32_t c;
	unsigned char *str = (unsigned char *)&number;
	for(size_t i = 0; i < sizeof(int32_t); i++)
	{
		c = *str++;
		out = c + (out << 6) + (out << 16) - out;
	}
	return out;
}