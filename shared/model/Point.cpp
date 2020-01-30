#include <Point.h>


Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

Point Point::getNullPoint()
{
	Point result;
	result.setToNaN();
	return result;
}

Point Point::fromAngle(float angle, float length)
{
	return Point(FMath::cos(angle) * length, FMath::sin(angle) * length);
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

void Point::scaleBy(float value)
{
	x *= value;
	y *= value;
}

void Point::setToNaN()
{
	x = std::numeric_limits<float>::quiet_NaN();
	y = std::numeric_limits<float>::quiet_NaN();
}

bool Point::isNaN() const
{
	return std::isnan(x) || std::isnan(y);
}

float Point::getLength() const
{
	return std::sqrt(x*x + y*y);
}

void Point::setFromAngle(float angle)
{
	x = FMath::cos(angle);
	y = FMath::sin(angle);
}

float Point::asAngle() const
{
	return FMath::atan2(y, x);
}

float Point::asAngleInDegrees() const
{
	return FMath::atan2(y, x) * 180 / M_PI;
}

std::string Point::toString() const
{
	std::stringstream stream;
	stream << "[" << std::fixed << std::setprecision(2) << x << ", " << std::fixed << std::setprecision(2) << y << "]";
	
	return stream.str();
}

std::string Point::toFullString() const
{
	std::stringstream stream;
	stream << "[" << std::fixed << std::setprecision(17) << x << ", " << std::fixed << std::setprecision(17) << y << "]";
	return stream.str();
}

float Point::distanceTo(const Point& anotherPoint) const
{
	return std::sqrt(
			(anotherPoint.x - x)*(anotherPoint.x - x) +
			(anotherPoint.y - y)*(anotherPoint.y - y)
	);
}

float Point::distanceTo(const float x, const float y) const
{
	return std::sqrt(
			(this->x - x)*(this->x - x) +
			(this->y - y)*(this->y - y)
	);
}

float Point::sqDistanceTo(const Point& anotherPoint) const
{
	return (anotherPoint.x - x)*(anotherPoint.x - x) +
			(anotherPoint.y - y)*(anotherPoint.y - y);
}

float Point::sqDistanceTo(const float x, const float y) const
{
	return (this->x - x)*(this->x - x) +
			(this->y - y)*(this->y - y);
}

Point Point::subtract(const Point& anotherPoint) const
{
	return Point(x - anotherPoint.x, y - anotherPoint.y);
}

void Point::subtract(const Point& anotherPoint, Point& out)
{
	out.x = x - anotherPoint.x;
	out.y = y - anotherPoint.y;
}

Point Point::add(const Point& anotherPoint) const
{
	return Point(x + anotherPoint.x, y + anotherPoint.y);
}

void Point::add(const Point& anotherPoint, Point& out)
{
	out.x = x + anotherPoint.x;
	out.y = y + anotherPoint.y;
}

Point Point::rotate(const float angle) const
{
	return Point(x * FMath::cos(angle) - y * FMath::sin(angle),  x * FMath::sin(angle) + y * FMath::cos(angle));
}

void Point::rotate(const float angle, Point& out)
{
	//&out can be the point itself -> don't mutate it until both x and y are final
	float nx = x * FMath::cos(angle) - y * FMath::sin(angle);
	float ny = x * FMath::sin(angle) + y * FMath::cos(angle);
	out.x = nx;
	out.y = ny;
}

float Point::crossProduct(const Point& anotherPoint) const
{
	return x*anotherPoint.y - anotherPoint.x * y;
}

float Point::cos(const Point& anotherPoint) const
{
	return (*this * anotherPoint) / (getLength() * anotherPoint.getLength());
}

float Point::sin(const Point& anotherPoint) const
{
	return crossProduct(anotherPoint) / (getLength() * anotherPoint.getLength());
}

float Point::projectOnto(const Point& anotherPoint) const
{
	return *this * anotherPoint / anotherPoint.getLength();
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

	
void Point::operator*=(const float& p)
{
	x *= p;
	y *= p;
}

void Point::operator/=(const float& p)
{
	x /= p;
	y /= p;
}

void Point::deserialize(SerializationStream& stream)
{
	Serializer::read(x, stream);
	Serializer::read(y, stream);
}

void Point::serialize(SerializationStream& stream) const
{
	Serializer::write(x, stream);
	Serializer::write(y, stream);
}

void from_json(const nlohmann::json& j, Point& point)
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
bool operator==(const Point& x, const Point& y)
{
	return x.x == y.x && x.y == y.y;
}
bool operator!=(const Point& x, const Point& y)
{
	return x.x != y.x || x.y != y.y;
}
float operator*(const Point& x, const Point& y)
{
	return x.x * y.x + x.y * y.y;
}
Point operator*(const Point& x, const float& y)
{
	return Point(x.x * y, x.y * y);
}
Point operator*(const float& x, const Point& y)
{
	return Point(y.x * x, y.y * x);
}
Point operator/(const Point& x, const float& y)
{
	return Point(x.x / y, x.y / y);
}