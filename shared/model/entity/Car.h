#pragma once
#include <ISerializable.h>
#include <CarProto.h>

class Car
{
public:
	Car();
	~Car();

	int16_t prototypeId;
	int32_t health;

	void init(CarProto &car);

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:

};
