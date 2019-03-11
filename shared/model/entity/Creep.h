#pragma once
#include <ISerializable.h>
#include <CarProto.h>
#include <BaseEntities.h>
#include <Point.h>

class Creep
{
public:
	Creep() = default;
	~Creep() = default;
	
	Object object;
	Unit unit;
	Weapon weapon;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};