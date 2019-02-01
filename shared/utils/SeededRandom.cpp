#include <SeededRandom.h>



SeededRandom::SeededRandom()
{
	std::random_device rd;
	initFromSeed(rd());
}

SeededRandom::SeededRandom(uint32_t seed)
{
	initFromSeed(seed);
}

void SeededRandom::initFromSeed(uint32_t seed)
{
	this->seed = seed;
}


SeededRandom::~SeededRandom()
{
}

int32_t SeededRandom::get(int32_t min, int32_t max)
{
	return (int32_t)(min + get() * (max - min));
}

int32_t SeededRandom::getInt()
{
	return get(0, 2147483647);
}

float SeededRandom::get(float min, float max)
{
	return min + get() * (max - min);
}

float SeededRandom::get()
{
	seed = (uint32_t)(((uint64_t)48271 * seed) % 2147483647);
	return seed / 2147483647.f;
}

void SeededRandom::deserialize(SerializationStream & stream)
{
	Serializer::read(seed, stream);
}

void SeededRandom::serialize(SerializationStream & stream)
{
	Serializer::write(seed, stream);
}
