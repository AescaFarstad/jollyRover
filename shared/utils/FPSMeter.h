#include <string>
#include <math.h>
#include <sstream>
#include <iomanip>

class FPSMeter
{
public:
	FPSMeter();
	~FPSMeter() = default;
	
	size_t frameCount();
	
	void registerFrame(size_t ticks);
	std::string report();
	size_t getMeasurementDuration();
	
private:
	size_t m_frameCount;
	size_t m_lastMeasurement;
	size_t m_lastCount;
	size_t m_time;
};


