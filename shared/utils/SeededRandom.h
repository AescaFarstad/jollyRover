#pragma once
#include <random>
#include <cstdint>

class SeededRandom
{
public:
	SeededRandom();
	SeededRandom(uint32_t m_seed);

	void initFromSeed(uint32_t m_seed);
	int32_t get(int32_t min, int32_t max);
	int32_t getInt();
	float get(float min, float max);
	float get();
	float getAngle();
	float peekNext();
	uint32_t getSeed() const;
	
	template <typename T>
	T& getFromVector(std::vector<T>& vec)
	{
		return vec[std::floor(get() * vec.size())];
	}

private:
	uint32_t m_seed;

};

#include <Serialization.h>

namespace Serialization
{
	
	//SeededRandom-------------------------------------------------------
	
	template <typename T>
	void write(const SeededRandom& object, T& serializer)
	{
		serializer.write(object.getSeed(), FIELD_NAME(seed));
	}
	
	template <typename T>
	void read(SeededRandom& object, T& serializer)
	{
		uint32_t seed;
		serializer.read(seed, FIELD_NAME(seed));
		object.initFromSeed(seed);
	}
}