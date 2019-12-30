#pragma once
#include <ISerializable.h>
#include <Point.h>

enum class ENTTITY_TYPE : int8_t
{
	CREEP,
	CAR
};

class Host
{
public:
	void* pointer;
	ENTTITY_TYPE type;
};

class Unit
{
public:
	int32_t id;
	int16_t prototypeId;
	int16_t force;
	int32_t health;
	Point location;
	Point voluntaryMovement;
	
	Host host_;
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};

namespace Serializer {

	void write(const ENTTITY_TYPE& value, SerializationStream& stream);
	void read(ENTTITY_TYPE& value, SerializationStream& stream);
}

class Target
{
public:
	int32_t id;
	ENTTITY_TYPE type;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};


class Weapon
{
public:
	int16_t prototypeId;
	int32_t attackCooldown;
	Target target;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};