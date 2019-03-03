#pragma once
#include <cstdint>
#include <algorithm> 

class TimeSync
{
public:
	TimeSync();
	~TimeSync();

	void addMeasurement(uint32_t localRequest, uint32_t serverResponse, uint32_t localResponse);
	int64_t localToServerUpperBound(uint32_t local);
	int64_t localToServerLowerBound(uint32_t local);
	int64_t serverToLocalUpperBound(uint32_t local);
	int64_t serverToLocalLowerBound(uint32_t local);
	int32_t getUncertainty();

private:
	int measurementCount;
	int64_t maxDelta;
	int64_t minDelta;
};
