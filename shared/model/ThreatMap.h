#include <Point.h>
#include <vector>

class ThreatMap
{
public:
	ThreatMap();
	ThreatMap(int32_t cellSize, Point AA, Point BB);
	
	void addThreat(Point& origin, int32_t value);
	void blur(float amount);
	void reset();
	bool isValid();
	int32_t getThreatAt(Point& origin);
	int32_t getTotalThreatAt(Point& origin, int32_t radius);
	
	int32_t getCellSize();
	const Point getAA();
	const std::vector<std::vector<int32_t>>* getRawData();
	
private:
	bool m_isValid;
	int32_t m_cellSize;
	Point m_AA;
	Point m_BB;
	std::vector<std::vector<int32_t>> m_map;
};
