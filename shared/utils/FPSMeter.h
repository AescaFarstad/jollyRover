#include <string>
#include <math.h>
#include <sstream>
#include <iomanip>

class FPSMeter
{
public:
	FPSMeter();
	~FPSMeter() = default;
	
	size_t frameCount;
	
	void registerFrame(size_t ticks);
	std::string report();
	size_t getMeasurementDuration();
	
private:
	size_t lastMeasurement;
	size_t lastCount;;
	size_t time;
};


