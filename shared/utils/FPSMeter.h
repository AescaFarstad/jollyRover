#pragma once
#include <string>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <CircularContainer.h>

class FPSMeter
{
public:
	FPSMeter();
	
	size_t frameCount();
	
	void registerFrame(size_t ticks);
	std::string report();
	float getfps(size_t duration);
	size_t getMeasurementDuration();
	
private:
	size_t m_frameCount;
	size_t m_lastMeasurement;
	size_t m_lastCount;
	size_t m_time;
	
	CircularContainer<size_t, 180>  m_ticks;
};


namespace S
{
	extern FPSMeter fpsMeter;
};

