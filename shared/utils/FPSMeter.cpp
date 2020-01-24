#include <FPSMeter.h>


FPSMeter::FPSMeter()
{
	m_frameCount = 0;
	m_lastMeasurement = 0;
	m_time = 0;
	m_lastCount = 0;
}

size_t FPSMeter::frameCount()
{
	return m_frameCount;
}

void FPSMeter::registerFrame(size_t ticks)
{
	m_frameCount++;
	m_time = ticks;
	if (m_lastMeasurement == 0)
		m_lastMeasurement = m_time;
	m_ticks.add(ticks);
}
std::string FPSMeter::report()
{
	double fps = (double)(m_frameCount - m_lastCount) * 1000 / (double)(m_time - m_lastMeasurement);
	
	std::stringstream stream;
	stream << "FPS (over " << std::setprecision(2) << (m_time - m_lastMeasurement) << "): " << fps << "\n";
	m_lastMeasurement = m_time;
	m_lastCount = m_frameCount;
	return stream.str();
}

float FPSMeter::getfps(size_t duration)
{
	auto iter = m_ticks.end() - 1;
	
	while(iter != m_ticks.begin() && *iter > m_time - duration)
		iter--;
	
	if (iter == m_ticks.begin())
		return -1;
	
	return (float)(CircularContainer<size_t, 180>::diff(m_ticks.end(), iter) - 1) * 1000 / (m_time - (*iter));
}

size_t FPSMeter::getMeasurementDuration()
{
	return (m_time - m_lastMeasurement);
}

namespace S
{
	FPSMeter fpsMeter;
}