#include <FPSMeter.h>


FPSMeter::FPSMeter()
{
	frameCount = 0;
	lastMeasurement = 0;
	time = 0;
	lastCount = 0;
}


void FPSMeter::registerFrame(size_t ticks)
{
	frameCount++;
	time = ticks;
	if (lastMeasurement == 0)
		lastMeasurement = time;
}
std::string FPSMeter::report()
{
	double fps = (double)(frameCount - lastCount) * 1000 / (double)(time - lastMeasurement);
	
	std::stringstream stream;
	stream << "FPS (over " << std::setprecision(2) << (time - lastMeasurement) << "): " << fps << "\n";
	lastMeasurement = time;
	lastCount = frameCount;
	return stream.str();
}

size_t FPSMeter::getMeasurementDuration()
{
	return (time - lastMeasurement);
}