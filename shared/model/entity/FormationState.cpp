#include <FormationState.h>

void FormationState::deserialize(SerializationStream &stream)
{
	Serializer::read(id, stream);
	Serializer::read(prototypeId, stream);
	Serializer::read(objectiveID, stream);
	Serializer::read(force, stream);
	Serializer::read(orientation, stream);
	Serializer::read(targetOrientation, stream);
	Serializer::read(location, stream);
	Serializer::read(targetLocation, stream);
	Serializer::read(speed, stream);
	Serializer::read(angularSpeed, stream);
	Serializer::read(spawnedAt, stream);
	Serializer::read(subObjective, stream);
	Serializer::read(agroAt, stream);
	Serializer::read(bravery, stream);
	Serializer::read(carAgro, stream);
	Serializer::readVector(slots, stream);
	
	formationPrototype_ = nullptr;
	isDisposed_ = false;
}

void FormationState::serialize(SerializationStream &stream) const
{
	Serializer::write(id, stream);
	Serializer::write(prototypeId, stream);
	Serializer::write(objectiveID, stream);
	Serializer::write(force, stream);
	Serializer::write(orientation, stream);
	Serializer::write(targetOrientation, stream);
	Serializer::write(location, stream);
	Serializer::write(targetLocation, stream);
	Serializer::write(speed, stream);
	Serializer::write(angularSpeed, stream);
	Serializer::write(spawnedAt, stream);
	Serializer::write(subObjective, stream);
	Serializer::write(agroAt, stream);
	Serializer::write(bravery, stream);
	Serializer::write(carAgro, stream);
	Serializer::writeVector(slots, stream);
}


void FormationState::propagatePrototypes(std::vector<FormationProto>& formations)
{
	formationPrototype_ = &(formations[prototypeId]);
	
}

void Serializer::write(const SUB_OBJECTIVE& value, SerializationStream& stream)
{
	Serializer::write((int8_t)value, stream);
}

void Serializer::read(SUB_OBJECTIVE& value, SerializationStream& stream)
{
	int8_t t;
	Serializer::read(t, stream);
	value = (SUB_OBJECTIVE)t;
}