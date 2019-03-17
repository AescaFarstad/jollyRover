#pragma once
#include <ISerializable.h>
#include <json.hpp>
#include <math.h>
#include <string>
#include <FMath.h>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

class Point
{
public:
	Point(float x = 0, float y = 0);
	~Point();

	float x;
	float y;

	void scaleTo(float length);
	void setTo(const Point &anotherPoint);
	void setToNaN();
	float getLength();
	float asAngle();
	std::string toString();

	float distanceTo(const Point& anotherPoint);
	Point subtract(const Point& anotherPoint);
	void subtract(const Point& anotherPoint, Point &out);
	Point add(const Point& anotherPoint);
	void add(const Point& anotherPoint, Point &out);
	Point rotate(float angle);
	void rotate(float angle, Point &out);
	float crossProduct(Point &anotherPoint);
	
	void operator+=(const Point& p);
	void operator-=(const Point& p);


	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};

void from_json(const json& j, Point& point);
Point operator+(const Point& x, const Point& y);
Point operator-(const Point& x, const Point& y);