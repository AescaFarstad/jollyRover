#pragma once
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
	

private:

};

class Target
{
public:
	int32_t id;
	ENTTITY_TYPE type;
};


class Weapon
{
public:
	int16_t prototypeId;
	int32_t attackCooldown;
	Target target;
};

#include <Serialization.h>

namespace Serialization
{
	
	//Unit-------------------------------------------------------
	
	template <typename T>
	void write(const Unit& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, id);
		WRITE_FIELD(object, serializer, prototypeId);
		WRITE_FIELD(object, serializer, force);
		WRITE_FIELD(object, serializer, health);
		WRITE_FIELD(object, serializer, location);
		WRITE_FIELD(object, serializer, voluntaryMovement);
	}
	
	template <typename T>
	void read(Unit& object, T& serializer)
	{
		READ__FIELD(object, serializer, id);
		READ__FIELD(object, serializer, prototypeId);
		READ__FIELD(object, serializer, force);
		READ__FIELD(object, serializer, health);
		READ__FIELD(object, serializer, location);
		READ__FIELD(object, serializer, voluntaryMovement);
	}
	
	//Weapon-------------------------------------------------------
	
	template <typename T>
	void write(const Weapon& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, prototypeId);
		WRITE_FIELD(object, serializer, attackCooldown);
		WRITE_FIELD(object, serializer, target);
	}
	
	template <typename T>
	void read(Weapon& object, T& serializer)
	{
		READ__FIELD(object, serializer, prototypeId);
		READ__FIELD(object, serializer, attackCooldown);
		READ__FIELD(object, serializer, target);
	}
	
	
	//Target-------------------------------------------------------
	
	template <typename T>
	void write(const Target& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, id);
		WRITE_FIELD(object, serializer, type);
	}
	
	template <typename T>
	void read(Target& object, T& serializer)
	{
		READ__FIELD(object, serializer, id);
		READ__FIELD(object, serializer, type);
	}
	
	
	//ENTTITY_TYPE---------------------------------------------------
	
	template <typename T>
	void write(const ENTTITY_TYPE& object, T& serializer)
	{
		serializer.write((int8_t)object, FIELD_NAME(ENTTITY_TYPE));
	}
	
	template <typename T>
	void read(ENTTITY_TYPE& object, T& serializer)
	{
		int8_t tmp;
		serializer.read(tmp, FIELD_NAME(ENTTITY_TYPE));
		object = (ENTTITY_TYPE)tmp;
	}
}