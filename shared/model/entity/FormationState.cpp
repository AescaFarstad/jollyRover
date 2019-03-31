#include <FormationState.h>

void FormationState::deserialize(SerializationStream &stream)
{
	Serializer::read(object, stream);
	Serializer::read(objectiveID, stream);
	Serializer::read(force, stream);
	Serializer::read(orientation, stream);
	Serializer::read(targetOrientation, stream);
	Serializer::read(location, stream);
	Serializer::read(targetLocation, stream);
	Serializer::read(speed, stream);
	Serializer::read(speedMulti, stream);
	Serializer::read(angularSpeed, stream);
	Serializer::readVector(slots, stream);
	formationPrototype_ = nullptr;
	objectivePrototype_ = nullptr;
	isDisposed_ = false;
}

void FormationState::serialize(SerializationStream &stream) const
{
	Serializer::write(object, stream);
	Serializer::write(objectiveID, stream);
	Serializer::write(force, stream);
	Serializer::write(orientation, stream);
	Serializer::write(targetOrientation, stream);
	Serializer::write(location, stream);
	Serializer::write(targetLocation, stream);
	Serializer::write(speed, stream);
	Serializer::write(speedMulti, stream);
	Serializer::write(angularSpeed, stream);
	Serializer::writeVector(slots, stream);
}


void FormationState::propagatePrototypes(std::vector<FormationProto>& formations, std::vector<std::vector<ObjectiveProto>>& objectives)
{
	formationPrototype_ = &(formations[object.prototypeId]);
	if (force >= 0 && objectiveID >= 0)
		objectivePrototype_ = &(objectives[force][objectiveID]);
	else
		objectivePrototype_ = nullptr;
	
}