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
	/*
	~Point() = default;
	Point(const Point&) = default;
	Point& operator=(const Point&) = default;*/
	
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

	float distanceTo(const Point& anotherPoint) const;
	float distanceTo(const float x, const float y) const;
	float sqDistanceTo(const Point& anotherPoint) const;
	float sqDistanceTo(const float x, const float y) const;
	Point subtract(const Point& anotherPoint) const;
	void subtract(const Point& anotherPoint, Point &out);
	Point add(const Point& anotherPoint) const;
	void add(const Point& anotherPoint, Point &out);
	Point rotate(const float angle) const;
	void rotate(const float angle, Point &out);
	float crossProduct(const Point &anotherPoint) const;
	float cos(const Point &anotherPoint) const;
	float sin(const Point &anotherPoint) const;
	
	void operator+=(const Point& p);
	void operator-=(const Point& p);
	void operator*=(const float& p);
	void operator/=(const float& p);


	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

};

void from_json(const json& j, Point& point);
Point operator+(const Point& x, const Point& y);
Point operator-(const Point& x, const Point& y);
bool operator==(const Point& x, const Point& y);
bool operator!=(const Point& x, const Point& y);
float operator*(const Point& x, const Point& y);
Point operator*(const Point& x, const float& y);
Point operator*(const float& x, const Point& y);