#include <ThreatMap.h>
#include <utility>

ThreatMap::ThreatMap()
{
	m_isValid = false;
}

ThreatMap::ThreatMap(int32_t cellSize, Point AA, Point BB)
{
	m_isValid = true;
	m_cellSize = cellSize;
	m_AA = AA;
	m_BB = BB;
	
	m_map.resize(std::ceil((BB.x - AA.x) / m_cellSize) + 2);
	for(auto& col : m_map)
		col.resize(std::ceil((BB.y - AA.y) / m_cellSize) + 2);
}

	
void ThreatMap::addThreat(Point& origin, int32_t value)
{
	if (origin.x < m_AA.x || origin.y < m_AA.y ||
		origin.x > m_BB.x || origin.y > m_BB.y)
	{
		return;
	}
	
	m_map[(origin.x - m_AA.x) / m_cellSize + 1][(origin.y - m_AA.y) / m_cellSize + 1] += value;
}

void ThreatMap::blur(float amount)
{
	auto before = m_map;
	for(size_t i = 1; i < m_map.size() - 1; i++)
	{
		for(size_t j = 1; j < m_map[i].size() - 1; j++)
		{
			m_map[i][j] += amount / std::sqrt(2) * before[i - 1][j - 1];
			m_map[i][j] += amount * before[i - 1][j];
			m_map[i][j] += amount / std::sqrt(2) * before[i - 1][j + 1];
			
			m_map[i][j] += amount * before[i][j - 1];			
			m_map[i][j] += amount * before[i][j + 1];
			
			m_map[i][j] += amount / std::sqrt(2) * before[i + 1][j - 1];
			m_map[i][j] += amount * before[i + 1][j];
			m_map[i][j] += amount / std::sqrt(2) * before[i + 1][j + 1];
			
			
			m_map[i][j] /= amount * (4 / std::sqrt(2) + 4) + 1;
		}
	}
}

void ThreatMap::reset()
{
	for(auto& col : m_map)
	for(auto& row : col)
		row *= 0;
}

int32_t ThreatMap::getThreatAt(Point& origin)
{
	if (!m_isValid)
		THROW_FATAL_ERROR("Threat map is not valid");
		
	
	if (origin.x < m_AA.x || origin.y < m_AA.y ||
		origin.x > m_BB.x || origin.y > m_BB.y)
	{
		return 0;
	}
	
	return m_map[(origin.x - m_AA.x) / m_cellSize + 1][(origin.y - m_AA.y) / m_cellSize + 1];
}

int32_t ThreatMap::getTotalThreatAt(Point& origin, int32_t radius)
{	
	if (!m_isValid)
		THROW_FATAL_ERROR("Threat map is not valid");
		
	float result = 0;	
		
	int32_t fromX = std::max(0.0f, (origin.x - radius - m_AA.x) / m_cellSize) + 1;
	int32_t fromY = std::max(0.0f, (origin.y - radius - m_AA.y) / m_cellSize) + 1;
	int32_t toX = std::min((float)m_map.size() - 2,    std::ceil((origin.x + radius - m_AA.x) / m_cellSize)) + 1;
	int32_t toY = std::min((float)m_map[0].size() - 2, std::ceil((origin.y + radius - m_AA.y) / m_cellSize)) + 1;
	
	Point normalizedOrigin = origin - m_AA;
	float buffer = std::sqrt(2) * m_cellSize / 2;
	float outDistance = (radius + buffer)*(radius + buffer);
	float inDistance = (radius - buffer)*(radius - buffer);
	if (buffer > radius)
		inDistance = 0;
	
	for(int32_t x = fromX; x <= toX; x++)
	{
		for(int32_t y = fromY; y <= toY; y++)
		{
			int32_t distance = normalizedOrigin.sqDistanceTo((x + 0.5) * m_cellSize, (y + 0.5) * m_cellSize);
			
			if (distance > outDistance)
				continue;
			else if (distance < inDistance)
				result += m_map[x][y];
			else 
				result += FMath::lerp(inDistance, 1.f, outDistance, 0.f, distance);
		}
	}
	return result;
}

bool ThreatMap::isValid()
{
	return m_isValid;
}


int32_t ThreatMap::getCellSize()
{
	return m_cellSize;
}

const Point ThreatMap::getAA()
{
	return m_AA;
}

const std::vector<std::vector<int32_t>>* ThreatMap::getRawData()
{
	return &m_map;
}