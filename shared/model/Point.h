#pragma once
#include <Global.h>
#include <json.hpp>
#include <math.h>
#include <string>
#include <FMath.h>
#include <iomanip>
#include <sstream>

class Point
{
public:
	Point(float x = 0, float y = 0);
	
	static Point getNullPoint();
	static Point fromAngle(float angle, float length = 1);

	float x;
	float y;

	void scaleTo(float length);
	void scaleBy(float value);
	void setToNaN();
	bool isNaN() const;
	float getLength() const;
	void setFromAngle(float angle);
	float asAngle() const;
	float asAngleInDegrees() const;
	std::string toString() const;
	std::string toFullString() const;

	[[nodiscard]] 	float distanceTo(const Point& anotherPoint) const;
	[[nodiscard]] 	float distanceTo(const float x, const float y) const;
	[[nodiscard]] 	float sqDistanceTo(const Point& anotherPoint) const;
	[[nodiscard]] 	float sqDistanceTo(const float x, const float y) const;
	[[nodiscard]] 	Point subtract(const Point& anotherPoint) const;
					void subtract(const Point& anotherPoint, Point& out);
	[[nodiscard]]	Point add(const Point& anotherPoint) const;
					void add(const Point& anotherPoint, Point& out);
	[[nodiscard]]	Point rotate(const float angle) const;
					void rotate(const float angle, Point& out);
	[[nodiscard]] 	float crossProduct(const Point& anotherPoint) const;
	[[nodiscard]]	float cos(const Point& anotherPoint) const;
	[[nodiscard]]	float sin(const Point& anotherPoint) const;
	[[nodiscard]]	float projectOnto(const Point& anotherPoint) const;
	
	void operator+=(const Point& p);
	void operator-=(const Point& p);
	void operator*=(const float& p);
	void operator/=(const float& p);


};

void from_json(const nlohmann::json& j, Point& point);
Point operator+(const Point& x, const Point& y);
Point operator-(const Point& x, const Point& y);
bool operator==(const Point& x, const Point& y);
bool operator!=(const Point& x, const Point& y);
float operator*(const Point& x, const Point& y);
Point operator*(const Point& x, const float& y);
Point operator*(const float& x, const Point& y);
Point operator/(const Point& x, const float& y);

#include <Serialization.h>

namespace Serialization
{
	
	//Point-------------------------------------------------------
	
	template <typename T>
	void write(const Point& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, x);
		WRITE_FIELD(object, serializer, y);
	}
	
	template <typename T>
	void read(Point& object, T& serializer)
	{
		READ__FIELD(object, serializer, x);
		READ__FIELD(object, serializer, y);
	}
}