#pragma once

#include <Global.h>
#include <BinarySerializer.h>
#include <CircularContainer.h>

constexpr int32_t CAPASITY = 200;

class CRCAccumulator
{
public:
	CRCAccumulator();
	void init(int32_t step);
	void add(uint32_t stamp, std::string crc);
	int32_t getNumNewEntries();
	int32_t getCapacity();
	std::string trace();
	std::vector<std::pair<uint32_t, std::string>> extract(int32_t count);
	std::optional<uint32_t> getMismatch(const std::vector<std::pair<uint32_t, std::string>>& data);
	
private:
	int32_t m_numEntries;
	int32_t m_step;
	CircularContainer<std::pair<uint32_t, std::string>, CAPASITY + 1> crcs;
	
};
