#pragma once
#include <Point.h>
#include <vector>
#include <unordered_map>
#include <utility>


/*
Представляет собой четыре сетки, каждая со своим сдвигом. ([0; 0] [1/2; 0] [0; 1/2] [1/2; 1/2])
При запросе определяется наиболее подходящая из них согласно эвристике: та, у которой выбранный радиус задевает наименьшее количество ячеек по вертикали и горизонтали. В зависимости от радиуса, это либо сетка, в которой исходная точка ближе всего к центру одной из ячеек, либо наоборот - дальше всего от центра.

Всё, что не попало в сетку, кидается в m_rest. Предполагается, что это единичные случаи.

Дефолтный конструктор создаёт нерабочую сетку, ибо дефолтный конструктор нужен для удобства, но без параметров работать на самом деле не будет.



*/

template <typename T>
class SpatialMap
{
public:
	SpatialMap<T>();
	SpatialMap<T>(int32_t cellSize, bool useLayeredMap, Point AA, Point BB);
	~SpatialMap() = default;	
	SpatialMap<T>& operator=(SpatialMap<T>&& that);	
	SpatialMap<T>& operator=(SpatialMap<T>& that) = delete;
	SpatialMap<T>(SpatialMap<T>&& that) = delete;
	SpatialMap<T>(SpatialMap<T>& that) = delete;
	
	
	void setUnique(std::vector<T>& data);
	void setNonUnique(std::vector<T>& data);
	std::vector<T*> getInRadius(Point& origin, int32_t radius);
	std::vector<T*> getInCell(Point& origin);
	std::vector<std::pair<Point, std::vector<T*>*>> getCellsInRadius(Point& origin, int32_t radius, float in_out_ratio = 0.5);
	template <typename F>
	std::vector<T*> getNearest(Point& origin, F predicate);
	int32_t getCellSize();
	bool isValid();
	
private:
	std::vector<std::vector<std::vector<T*>>> m_map[2][2]; //[gx][gy][x][y]items[]
	Point m_gridOffset[2][2]; //[gx][gy]
	std::unordered_map<std::vector<std::vector<std::vector<T*>>>*, Point*> m_offsetByMap;
	std::vector<T*> m_rest;
	float m_safetyBuffer;
	int32_t m_totalSize;
	int32_t m_cellSize;
	Point m_AA;
	Point m_BB;
	Point m_dimensions;
	bool m_useLayeredMap;
	bool m_isValid;
	
	std::vector<std::vector<std::vector<T*>>>& findBestMap(Point& origin, int32_t radius);
	void clearMap(std::vector<std::vector<std::vector<T*>>>& map, int32_t expectedCount);
	
};

template <typename T>
SpatialMap<T>& SpatialMap<T>::operator=(SpatialMap<T>&& that)
{
	m_isValid = that.m_isValid;
	
	m_map[0][0] = std::move(that.m_map[0][0]);
	m_map[1][0] = std::move(that.m_map[1][0]);
	m_map[0][1] = std::move(that.m_map[0][1]);
	m_map[1][1] = std::move(that.m_map[1][1]);
	
	m_gridOffset[0][0] = that.m_gridOffset[0][0];
	m_gridOffset[1][0] = that.m_gridOffset[1][0];
	m_gridOffset[0][1] = that.m_gridOffset[0][1];
	m_gridOffset[1][1] = that.m_gridOffset[1][1];
	
	m_offsetByMap[&m_map[0][0]] = &m_gridOffset[0][0];
	m_offsetByMap[&m_map[1][0]] = &m_gridOffset[1][0];
	m_offsetByMap[&m_map[0][1]] = &m_gridOffset[0][1];
	m_offsetByMap[&m_map[1][1]] = &m_gridOffset[1][1];
	
	m_rest = std::move(that.m_rest);
	
	m_safetyBuffer = that.m_safetyBuffer;
	m_totalSize = that.m_totalSize;
	m_cellSize = that.m_cellSize;
	m_AA = std::move(that.m_AA);
	m_BB = std::move(that.m_BB);
	m_dimensions = std::move(that.m_dimensions);
	m_useLayeredMap = std::move(that.m_useLayeredMap);
	
	return *this;
}

template <typename T>
void traceMap(std::vector<std::vector<std::vector<T*>>>& map, std::vector<T*>& m_rest)
{
	std::cout << map.size() << "\n";
	for(auto& m : map)
	{
		std::cout << "\t" << m.size() << "\n";
		for(auto& n : m)
		{
			if (!n.size())
				continue;
			//std::cout << "\t\t" << n.size() << "\n";
			std::string tmp = "";
			for(auto& r : n)
			{
				tmp += std::to_string(r->object.prototypeId) + ",";
			}
			std::cout << "\t\t" << n.size() << " " << tmp << "\n";
		}
	}
	
	std::string tmp = "rest:";
	for(auto& m : m_rest)
	{
		tmp += std::to_string(m->object.prototypeId) + ",";
	}
	std::cout << "\t\t" << m_rest.size() << " " << tmp << "\n";
};

template <typename T>
SpatialMap<T>::SpatialMap()
{
	m_isValid = false;
	
}

template <typename T>
SpatialMap<T>::SpatialMap(int32_t cellSize, bool useLayeredMap, Point AA, Point BB)
{
	m_isValid = true;
	m_cellSize = cellSize;
	m_useLayeredMap = useLayeredMap;
	m_dimensions = BB - AA;
	m_AA = AA;
	m_AA -= Point(cellSize/2, cellSize/2); //buffer
	m_BB = BB;
	m_BB += Point(cellSize/2, cellSize/2); //buffer
	
	m_gridOffset[0][0].x = 0;
	m_gridOffset[0][0].y = 0;
	
	m_gridOffset[0][1].x = 0;
	m_gridOffset[0][1].y = cellSize / 2;
	
	m_gridOffset[1][0].x = 0;
	m_gridOffset[1][0].y = cellSize / 2;
	
	m_gridOffset[1][1].x = cellSize / 2;
	m_gridOffset[1][1].y = cellSize / 2;
	
	m_offsetByMap[&m_map[0][0]] = &m_gridOffset[0][0];
	m_offsetByMap[&m_map[1][0]] = &m_gridOffset[1][0];
	m_offsetByMap[&m_map[0][1]] = &m_gridOffset[0][1];
	m_offsetByMap[&m_map[1][1]] = &m_gridOffset[1][1];
	
	//extend the size to be exactly divisible by cellSize
	int extraX = (int)(m_dimensions.x) % cellSize;
	if (extraX != 0)
	{
		int delta = cellSize - extraX;
		m_BB.x += delta;
		m_dimensions.x += delta;
	}
	
	int extraY = (int)(m_dimensions.y) % cellSize;
	if (extraY != 0)
	{
		int delta = cellSize - extraY;
		m_BB.y += delta;
		m_dimensions.y += delta;
	}
	
	for(size_t g = 0; g < 4; g++)
	{
		int32_t gx = g % 2;
		int32_t gy = g / 2;
		m_map[gx][gy].resize(m_dimensions.x / m_cellSize + 1);
		for(size_t x = 0; x < m_dimensions.x/ m_cellSize + 1; x++)
		{
			m_map[gx][gy][x].resize(m_dimensions.y / m_cellSize + 1);
		}
	}
	
	m_safetyBuffer = std::sqrt(2) * m_cellSize / 2;
	m_totalSize = 0;
}
template <typename T>
void SpatialMap<T>::clearMap(std::vector<std::vector<std::vector<T*>>>& map, int32_t expectedCount)
{
	for(auto& x : map)
	for(auto& y : x)
	{
		y.clear();
		y.reserve(expectedCount);
	}
	m_rest.clear();
	m_rest.reserve(expectedCount);
}

template <typename T>
void SpatialMap<T>::setUnique(std::vector<T>& data)
{
	if (!m_isValid)
		THROW_FATAL_ERROR("Spatial map is not valid.");
		
	float density = data.size() / (m_map[0][0].size() * m_map[0][0][0].size());
	density *= 2;
	clearMap(m_map[0][0], density);
	
	if (!m_useLayeredMap)
		density = 0;
		
	clearMap(m_map[1][0], density);
	clearMap(m_map[0][1], density);
	clearMap(m_map[1][1], density);	
	
	for(auto& iter : data)
	{		
		const Point& loc = iter.getLocation();
		if (loc.x < m_AA.x + m_cellSize / 2 || loc.y < m_AA.y + m_cellSize / 2 || loc.x >= m_BB.x || loc.y >= m_BB.y)
			m_rest.push_back(&iter);
		else
		{
			size_t numMaps = m_useLayeredMap ? 4 : 1;
			for(size_t g = 0; g < numMaps; g++)
			{
				int32_t gx = g % 2;
				int32_t gy = g / 2;
				int32_t x = (loc.x - m_AA.x - m_gridOffset[gx][gy].x) / m_cellSize;
				int32_t y = (loc.y - m_AA.y - m_gridOffset[gx][gy].y) / m_cellSize;
				m_map[gx][gy][x][y].push_back(&iter);
			}	
		}
		
	}
	m_totalSize = data.size();
}

template <typename T>
void SpatialMap<T>::setNonUnique(std::vector<T>& data)
{
	if (!m_isValid)
		THROW_FATAL_ERROR("Spatial map is not valid.");
		
	float density = data.size() / (m_map[0][0].size() * m_map[0][0][0].size());
	density *= 4;
	clearMap(m_map[0][0], density);
	
	if (!m_useLayeredMap)
		density = 0;
		
	clearMap(m_map[1][0], density);
	clearMap(m_map[0][1], density);
	clearMap(m_map[1][1], density);	
	
	Point vert[4] = {Point(0, 0), Point(0, m_cellSize), Point(m_cellSize, 0), Point(m_cellSize, m_cellSize)};
	Edge protoEdges[4] = {
		Edge(&vert[0], &vert[1]),
		Edge(&vert[0], &vert[2]),
		Edge(&vert[1], &vert[3]),
		Edge(&vert[2], &vert[3])
		};
	
	for(auto& iter : data)
	{
		const Point& AA = iter.getAA();
		const Point& BB = iter.getBB();
		const Point& CC = iter.getCenter();
		
		size_t numMaps = m_useLayeredMap ? 4 : 1;
		for(size_t g = 0; g < numMaps; g++)
		{
			int32_t gx = g % 2;
			int32_t gy = g / 2;
			
			auto& map = m_map[gx][gy];
			
			Point& offset = m_gridOffset[gx][gy];
			
			int32_t fromX = std::max(0.0f, (AA.x - m_AA.x - offset.x) / m_cellSize);
			int32_t fromY = std::max(0.0f, (AA.y -  m_AA.y - offset.y) / m_cellSize);
			
			int32_t toX = std::min((float)map.size() - 1,    std::ceil((BB.x - m_AA.x - offset.x) / m_cellSize));
			int32_t toY = std::min((float)map[0].size() - 1, std::ceil((BB.y - m_AA.y - offset.y) / m_cellSize));
			
			int32_t offsetX = - m_AA.x + offset.x;
			int32_t offsetY = - m_AA.y + offset.y;
			for(int32_t x = fromX; x <= toX; x++)
			{
				for(int32_t y = fromY; y <= toY; y++)
				{
					Point cellCenter((x + 0.5) * m_cellSize - offsetX, (y + 0.5) * m_cellSize - offsetY);
					if (iter.hitTest(cellCenter))
					{
						map[x][y].push_back(&iter);
						continue;
					}
					
					
					for(auto& edge : protoEdges)
					{
						Point fromP(x * m_cellSize - offsetX + edge.p1->x, y * m_cellSize - offsetY + edge.p1->y);
						Point toP(x * m_cellSize - offsetX + edge.p2->x, y * m_cellSize - offsetY + edge.p2->y);
						Edge testEdge(&fromP, &toP);
						
						if (iter.hitTest(testEdge))
						{
							map[x][y].push_back(&iter);
							goto edgeTestFinish;
						}
					}
					edgeTestFinish:{}/**/
				}
			}
					
			int32_t x = (CC.x - m_AA.x - m_gridOffset[gx][gy].x) / m_cellSize;
			int32_t y = (CC.y - m_AA.y - m_gridOffset[gx][gy].y) / m_cellSize;
			if (map[x][y].end() != std::find(map[x][y].begin(), map[x][y].end(), &iter))
				map[x][y].push_back(&iter);
		}	
	}
	m_totalSize = 0;
	for(auto&gx : m_map)
	for(auto&gy : gx)
	for(auto&x : gy)
	for(auto&y : x)
		m_totalSize+= y.size();	
}

template <typename T>
std::vector<std::vector<std::vector<T*>>>& SpatialMap<T>::findBestMap(Point& origin, int32_t radius)
{
	int32_t dx = (int)(origin.x - m_AA.x) % m_cellSize;
	int32_t dy = (int)(origin.y - m_AA.y) % m_cellSize;
	
	int32_t gx, gy;
	
	if (dx > m_cellSize / 4 && dx < m_cellSize * 3 / 4)
		gx = 0;
	else
		gx = 1;
		
	if (dy > m_cellSize / 4 && dy < m_cellSize * 3 / 4)
		gy = 0;
	else
		gy = 1;
	
	if (radius % m_cellSize > m_cellSize / 2)
	{
		gx = 1 - gx;
		gy = 1 - gy;
	}
	
	return m_map[gx][gy];
}


template <typename T>
std::vector<T*> SpatialMap<T>::getInRadius(Point& origin, int32_t radius)
{
	if (!m_isValid)
		THROW_FATAL_ERROR("Spatial map is not valid.");
		
	auto& map = m_useLayeredMap ? findBestMap(origin, radius) : m_map[0][0];
	
	Point* offset = m_offsetByMap[&map];
	
	int32_t fromX = std::max(0.0f, (origin.x - radius - m_AA.x - offset->x) / m_cellSize);
	int32_t fromY = std::max(0.0f, (origin.y - radius - m_AA.y - offset->y) / m_cellSize);
	int32_t toX = std::min((float)map.size() - 1,    std::ceil((origin.x + radius - m_AA.x - offset->x) / m_cellSize));
	int32_t toY = std::min((float)map[0].size() - 1, std::ceil((origin.y + radius - m_AA.y - offset->y) / m_cellSize));
	
	bool goesOutOfBounds = 
		(origin.x - radius < m_AA.x ||
		origin.x + radius >= m_BB.x ||
		origin.y - radius < m_AA.y ||
		origin.y + radius >= m_AA.y);
		
	std::vector<T*> result;
	int32_t expectedSize = (goesOutOfBounds ? 1.5 : 2) * radius * radius * M_PI * m_totalSize / (m_dimensions.x * m_dimensions.y);
	expectedSize = std::min(m_totalSize / 2, expectedSize);
	result.reserve(expectedSize);	
	
	Point normalizedOrigin = origin - m_AA - *offset;
	float outDistance = (radius + m_safetyBuffer)*(radius + m_safetyBuffer);
	float inDistance = (radius - m_safetyBuffer)*(radius - m_safetyBuffer);
	if (m_safetyBuffer > radius)
		inDistance = 0;
	int32_t sqRadius = radius * radius; 
	
	for(int32_t x = fromX; x <= toX; x++)
	{
		for(int32_t y = fromY; y <= toY; y++)
		{
			int32_t distance = normalizedOrigin.sqDistanceTo((x + 0.5) * m_cellSize, (y + 0.5) * m_cellSize);
			
			if (distance > outDistance)
				continue;
			else if (distance < inDistance)
				result.insert(result.end(), map[x][y].begin(), map[x][y].end());
			else 
			{
				for(auto& iter : map[x][y])
				{
					if (origin.sqDistanceTo(iter->getLocation()) < sqRadius)
						result.push_back(iter);
				}
			}
		}
	}
	
	if (goesOutOfBounds)
	{
		for(auto& iter : m_rest)
		{
			if (origin.sqDistanceTo(iter->getLocation()) < sqRadius)
				result.push_back(iter);
		}
	}
	
	return result;
}

template <typename T>
std::vector<T*> SpatialMap<T>::getInCell(Point& origin)
{
	if (!m_isValid)
		THROW_FATAL_ERROR("Spatial map is not valid.");
		
	if (origin.x < m_AA.x || origin.y < m_AA.y || origin.x > m_BB.x || origin.y > m_BB.y)
		return m_rest;
		
	auto& map = m_map[0][0];
	
	Point& offset = m_gridOffset[0][0];
	
	int32_t x = (origin.x - m_AA.x - offset.x) / m_cellSize;
	int32_t y = (origin.y - m_AA.y - offset.y) / m_cellSize;
	
	return map[x][y];
	
}

///Returns a pair: [cell center, cell content]. Does not return the put-of-bounds cell since it has no center.
template <typename T>
std::vector<std::pair<Point, std::vector<T*>*>> SpatialMap<T>::getCellsInRadius(Point& origin, int32_t radius, float in_out_ratio)
{
	if (!m_isValid)
		THROW_FATAL_ERROR("Spatial map is not valid.");
		
	auto& map = m_useLayeredMap ? findBestMap(origin, radius) : m_map[0][0];
	
	Point* offset = m_offsetByMap[&map];
	
	int32_t fromX = (origin.x - radius - m_AA.x - offset->x) / m_cellSize;
	int32_t fromY = (origin.y - radius - m_AA.y - offset->y) / m_cellSize;
	int32_t toX = std::ceil((origin.x + radius - m_AA.x - offset->x) / m_cellSize);
	int32_t toY = std::ceil((origin.y + radius - m_AA.y - offset->y) / m_cellSize);
	
	int32_t maxX = map.size() - 1;
	int32_t maxY = map[0].size() - 1;
	bool goesOffBounds = fromX < 0 || fromY < 0 || toX > maxX || toY > maxY;
	if (goesOffBounds)
	{
		fromX = std::min(std::max(0, fromX), maxX);
		fromY = std::min(std::max(0, fromY), maxY);
		toX = std::min(std::max(0, toX), maxX);
		toY = std::min(std::max(0, toY), maxY);
	}
	
	std::vector<std::pair<Point, std::vector<T*>*>> result;
	result.reserve((toX - fromX) * (toY - fromY));
	
	Point normalizedOrigin = origin - m_AA - *offset;
	float outDistance = (radius + m_safetyBuffer)*(radius + m_safetyBuffer);
	float inDistance = (radius - m_safetyBuffer)*(radius - m_safetyBuffer);
	float activeDistance = FMath::lerp(0, inDistance, 1, outDistance, in_out_ratio);
	
	for(int32_t x = fromX; x <= toX; x++)
	{
		for(int32_t y = fromY; y <= toY; y++)
		{
			Point cellCenter((x + 0.5) * m_cellSize, (y + 0.5) * m_cellSize);
			int32_t distance = normalizedOrigin.sqDistanceTo(cellCenter);
			
			if (distance > activeDistance)
				continue;
			else
				result.push_back(std::pair<Point, std::vector<T*>*>{cellCenter + m_AA + *offset , &map[x][y]});
		}
	}
	return result;
}


template <typename T>
int32_t SpatialMap<T>::getCellSize()
{
	return m_cellSize;
}

template <typename T>
bool SpatialMap<T>::isValid()
{
	return m_isValid;
}