#pragma once

#include <Global.h>
#include <BinarySerializer.h>
#include <CircularContainer.h>

class CRCAccumulator
{
public:
	CRCAccumulator();
	void init(int32_t step);
	void add(int32_t stamp, std::string crc);
	int32_t getNumNewEntries();
	std::unique_ptr<BinarySerializer> extract(int32_t count);
	
private:
	int32_t m_numEntries;
	int32_t m_step;
	CircularContainer<std::pair<int32_t, std::string>, 200> crcs;
	
};
