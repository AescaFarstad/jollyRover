#include <CreepState.h>


CreepState::CreepState()
{
	_creepProto = nullptr;
	_weaponProto = nullptr;
}

void CreepState::deserialize(SerializationStream &stream)
{
	Serializer::read(object, stream);
	Serializer::read(unit, stream);
	Serializer::read(weapon, stream);
	Serializer::read(mode, stream);
	Serializer::read(formationId, stream);
	Serializer::read(formationsSlot, stream);
	Serializer::read(velocity, stream);
	Serializer::read(orientation, stream);
	Serializer::read(sensedAnObstacle, stream);
	Serializer::read(numWhiskers, stream);
	for(int8_t i = 0; i < numWhiskers; i++)
		Serializer::read(whiskers[i], stream);
	_creepProto = nullptr;
	_weaponProto = nullptr;
}

void CreepState::serialize(SerializationStream &stream) const
{
	Serializer::write(object, stream);
	Serializer::write(unit, stream);
	Serializer::write(weapon, stream);
	Serializer::write(mode, stream);
	Serializer::write(formationId, stream);
	Serializer::write(formationsSlot, stream);
	Serializer::write(velocity, stream);
	Serializer::write(orientation, stream);
	Serializer::write(sensedAnObstacle, stream);
	Serializer::write(numWhiskers, stream);
	for(int8_t i = 0; i < numWhiskers; i++)
		Serializer::write(whiskers[i], stream);
		
}

void CreepState::propagatePrototypes(std::vector<CreepProto>& creepProtos, std::vector<WeaponProto>& weaponProtos)
{
	_creepProto = &(creepProtos[object.prototypeId]);
	_weaponProto = &(weaponProtos[_creepProto->weapon]);
	
}

const Point& CreepState::getLocation()
{
	return unit.location;
}


void Serializer::write(const CREEP_MODE& value, SerializationStream& stream)
{
	Serializer::write((int32_t)value, stream);
}
void Serializer::read(CREEP_MODE& value, SerializationStream& stream)
{
	int32_t t;
	Serializer::read(t, stream);
	value = (CREEP_MODE)t;
}