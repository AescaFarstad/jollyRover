#include <FormationState.h>

void FormationState::deserialize(SerializationStream &stream)
{
	Serializer::read(object, stream);
	Serializer::read(objectiveID, stream);
	Serializer::read(force, stream);
	Serializer::read(direction, stream);
	Serializer::readVector(slots, stream);
	_prototype = nullptr;
	_isDisposed = false;
}

void FormationState::serialize(SerializationStream &stream) const
{
	Serializer::write(object, stream);
	Serializer::write(objectiveID, stream);
	Serializer::write(force, stream);
	Serializer::write(direction, stream);
	Serializer::writeVector(slots, stream);
}

