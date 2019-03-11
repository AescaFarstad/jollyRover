#pragma once
#include <random>
#include <cstdint>
#include <ISerializable.h>

class SeededRandom
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
	
	template <typename T>
	T& getFromVector(std::vector<T> &vec)
	{
		return 	vec[std::floor(get() * vec.size())];
	}

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

private:
	uint32_t seed;

};

