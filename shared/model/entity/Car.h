#pragma once
#include <ISerializable.h>
#include <CarProto.h>

class Car : public ISerializable
{
public:
	Car();
	~Car();

	int16_t prototypeId;
	int32_t health;

	void init(CarProto &car);

	void deserialize(SerializationStream& stream) override;
	void serialize(SerializationStream& stream) override;

private:

};
