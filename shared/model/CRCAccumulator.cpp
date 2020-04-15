#include <CRCAccumulator.h>


CRCAccumulator::CRCAccumulator()
{
	m_numEntries = 0;
}

void CRCAccumulator::init(int32_t step)
{
	m_step = step;
	m_numEntries = 0;
	crcs = CircularContainer<std::pair<uint32_t, std::string>, CAPASITY + 1>();
}

void CRCAccumulator::add(uint32_t stamp, std::string crc)
{
	if (m_numEntries == 0 || (crcs.end() -1)->first < stamp)
	{
		crcs.add(std::pair<int32_t, std::string>(stamp, crc));
		m_numEntries = std::min(m_numEntries + 1, (int32_t)crcs.size - 1);
	}
	else
	{
		auto last = (crcs.end() -1)->first;
		int32_t numSteps = ((crcs.end() -1)->first - stamp) / m_step + 1;
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

int32_t CRCAccumulator::getCapacity()
{
	return CAPASITY;
}

std::string CRCAccumulator::trace()
{
	std::string result;
	for(auto& datum : crcs)
		result += std::to_string(datum.first) + "=" + datum.second + "\n";
	return result;		
}

std::vector<std::pair<uint32_t, std::string>>  CRCAccumulator::extract(int32_t count)
{
	std::vector<std::pair<uint32_t, std::string>> result;
	result.reserve(count);
	auto end = crcs.begin() + count;
	
	for(auto i = crcs.begin(); i != end; i++)
		result.push_back(*i);
		
	//result.insert(result.begin(), crcs.begin(), end);
	//S::log.add("Extract " + std::to_string(result.size()) +  " entries.");
	crcs.skipFirst(count);
	m_numEntries -= count;
	
	return result;
}

std::optional<uint32_t> CRCAccumulator::getMismatch(const std::vector<std::pair<uint32_t, std::string>>& data)
{
	int32_t numSteps = ((crcs.end() -1)->first - data[0].first) / m_step + 1;
	if (numSteps > m_numEntries)
	{
		S::log.add("Can't find crc mismatch because the entry is too old: " + 
			std::to_string(data[0].first) + " earliest entry is " + std::to_string(crcs.begin()->first), {LOG_TAGS::ERROR_});
		return {};
	}
	auto iter = crcs.end() - numSteps;
	for(auto& datum : data)
	{
		if (iter->first != datum.first)
		{
			S::log.add("crcs aren't aligned", {LOG_TAGS::ERROR_});
			return {};
		}
		if (iter->second != datum.second)
		{
			S::log.add("Desync details. At " + std::to_string(iter->first) + ": " + iter->second + " while clients says " + datum.second, {LOG_TAGS::ERROR_});
			return {iter->first};
		}
		iter++;
	}
	return {};
}
