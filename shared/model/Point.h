#pragma once
#include <ISerializable.h>
#include <json.hpp>
#include <math.h>
#include <string>
#include <FMath.h>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

class Point : public ISerializable
{
public:
	Point(float x = 0, float y = 0);
	virtual ~Point();

	float x;
	float y;

	void scaleTo(float length);
	void setTo(const Point &anotherPoint);
	void setToNaN();
	float getLength();
	float asAngle();
	std::string toString();

	float distanceTo(const Point &anotherPoint);
	Point subtract(const Point &anotherPoint);
	void subtract(const Point &anotherPoint, Point &out);
	Point add(const Point &anotherPoint);
	void add(const Point &anotherPoint, Point &out);


	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) override;

private:

};

void from_json(const json& j, Point& point);