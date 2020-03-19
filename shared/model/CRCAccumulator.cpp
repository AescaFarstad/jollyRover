#include <CRCAccumulator.h>


CRCAccumulator::CRCAccumulator()
{
	m_numEntries = 0;
}

void CRCAccumulator::init(int32_t step)
{
	m_step = step;
	m_numEntries = 0;
	crcs = CircularContainer<std::pair<int32_t, std::string>, 200>();
}

void CRCAccumulator::add(int32_t stamp, std::string crc)
{
	if (crcs.total == 0 || (crcs.end() -1)->first == stamp - m_step)
	{
		crcs.add(std::pair<int32_t, std::string>(stamp, crc));
		m_numEntries = std::min(m_numEntries + 1, (int32_t)crcs.size - 1);
	}
	else
	{
		auto numSteps = ((crcs.end() -1)->first - stamp) / m_step + 1;
		if (numSteps > m_numEntries)
		{
			S::log.add("new crc for an old(?) entry: " + std::to_string(stamp) + " = " + crc, {LOG_TAGS::ERROR_});
			return;
		}
		
		crcs.skipLast(numSteps);
		m_numEntries -= numSteps;
		add(stamp, crc);
	}
}

int32_t CRCAccumulator::getNumNewEntries()
{
	return m_numEntries;
}

std::unique_ptr<BinarySerializer> CRCAccumulator::extract(int32_t count)
{
	auto result = std::make_unique<BinarySerializer>();
	
	for(auto i = 0; i < count; i++)
	{
		auto& pair = *(crcs.begin() + i);
		result->write(pair.first);
		result->write(pair.second);
	}
	
	crcs.skipFirst(count);	
	
	return result;
}
