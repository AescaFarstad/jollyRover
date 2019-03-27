#include <Point.h>


Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Point::scaleTo(float length)
{
	if (length == 0)
	{
		x = 0;
		y = 0;
		return;
	}

	float myLength = getLength();
	if (myLength != 0)
	{
		x *= length / myLength;
		y *= length / myLength;
	}
	else
	{
		THROW_FATAL_ERROR("Can't scale (0;0) point");
	}
}

void Point::setToNaN()
{
	x = std::numeric_limits<float>::quiet_NaN();
	y = std::numeric_limits<float>::quiet_NaN();
}

float Point::getLength()
{
	return std::sqrt(x*x + y*y);
}

float Point::asAngle()
{
	return FMath::atan2(y, x);
}

std::string Point::toString()
{
	std::stringstream stream;
	stream << "[" << std::fixed << std::setprecision(2) << x << ", " << std::fixed << std::setprecision(2) << y << "]";
	
	return stream.str();
}

float Point::distanceTo(const Point &anotherPoint)
{
	return std::sqrt(
			(anotherPoint.x - x)*(anotherPoint.x - x) +
			(anotherPoint.y - y)*(anotherPoint.y - y)
	);
}

float Point::distanceTo(const float x, const float y)
{
	return std::sqrt(
			(this->x - x)*(this->x - x) +
			(this->y - y)*(this->y - y)
	);
}

float Point::sqDistanceTo(const Point &anotherPoint)
{
	return (anotherPoint.x - x)*(anotherPoint.x - x) +
			(anotherPoint.y - y)*(anotherPoint.y - y);
}

float Point::sqDistanceTo(const float x, const float y)
{
	return (this->x - x)*(this->x - x) +
			(this->y - y)*(this->y - y);
}

Point Point::subtract(const Point &anotherPoint)
{
	return Point(x - anotherPoint.x, y - anotherPoint.y);
}

void Point::subtract(const Point &anotherPoint, Point &out)
{
	out.x = x - anotherPoint.x;
	out.y = y - anotherPoint.y;
}

Point Point::add(const Point & anotherPoint)
{
	return Point(x + anotherPoint.x, y + anotherPoint.y);
}

void Point::add(const Point & anotherPoint, Point & out)
{
	out.x = x + anotherPoint.x;
	out.y = y + anotherPoint.y;
}

Point Point::rotate(const float angle)
{
	return Point(x * FMath::cos(angle) - y * FMath::sin(angle),  x * FMath::sin(angle) + y * FMath::cos(angle));
}

void Point::rotate(const float angle, Point &out)
{
	//&out can be the point itself -> don't mutate it until both x and y are final
	float nx = x * FMath::cos(angle) - y * FMath::sin(angle);
	float ny = x * FMath::sin(angle) + y * FMath::cos(angle);
	out.x = nx;
	out.y = ny;
}

float Point::crossProduct(const Point &anotherPoint)
{
	return x*anotherPoint.y - anotherPoint.x * y;
}
	
void Point::operator+=(const Point& p)
{
	x += p.x;
	y += p.y;
}

void Point::operator-=(const Point& p)
{
	x -= p.x;
	y -= p.y;
}

void Point::deserialize(SerializationStream &stream)
{
	Serializer::read(x, stream);
	Serializer::read(y, stream);
}

void Point::serialize(SerializationStream &stream) const
{
	Serializer::write(x, stream);
	Serializer::write(y, stream);
}

void from_json(const json &j, Point &point)
{
	point.x = j.at("x");
	point.y = j.at("y");
}

Point operator+(const Point& x, const Point& y)
{
	return Point(x.x + y.x, x.y + y.y);
}
Point operator-(const Point& x, const Point& y)
{
	return Point(x.x - y.x, x.y - y.y);
}