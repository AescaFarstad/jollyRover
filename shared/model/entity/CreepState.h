#pragma once
#include <ISerializable.h>
#include <BaseEntities.h>
#include <CreepProto.h>
#include <Point.h>

enum class CREEP_MODE : int32_t
{
	FORMATION = 1,
	ASSAULT = 2,
	RETREAT = 3
};

class CreepState
{
public:
	CreepState();
	~CreepState() = default;
	
	Object object;
	Unit unit;
	Weapon weapon;
	CREEP_MODE mode;
	
	CreepProto* _creepProto;
	WeaponProto* _weaponProto;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};


namespace Serializer {

	void write(const CREEP_MODE& value, SerializationStream& stream);
	void read(CREEP_MODE& value, SerializationStream& stream);
}