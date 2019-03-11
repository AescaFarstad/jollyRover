#pragma once
#include <ISerializable.h>
#include <Point.h>

class Object
{
public:
	Object() = default;
	~Object() = default;

	int32_t id;
	int16_t prototypeId;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};

class Unit
{
public:
	Unit() = default;
	~Unit() = default;

	int16_t force;
	int32_t health;
	Point location;
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};


class Weapon
{
public:
	Weapon() = default;
	~Weapon() = default;

	int16_t prototypeId;
	int32_t attackCooldown;
	int32_t target;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};