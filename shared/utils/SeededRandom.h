#pragma once
#include <random>
#include <cstdint>
#include <ISerializable.h>

class SeededRandom :
	public ISerializable
{
public:
	SeededRandom();
	SeededRandom(uint32_t seed);
	~SeededRandom();


	void initFromSeed(uint32_t seed);
	int32_t get(int32_t min, int32_t max);
	int32_t getInt();
	float get(float min, float max);
	float get();

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);

private:
	uint32_t seed;

};

