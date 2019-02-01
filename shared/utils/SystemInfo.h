#pragma once
#include <cstdint>
class SystemInfo
{
public:
	SystemInfo();
	~SystemInfo();
	static SystemInfo* instance;

	bool isBigEndian = testIsBigEndian();

private:

	static bool testIsBigEndian()
	{
		int16_t test = 1;
		return  (*((char*)&test)) == 0;
	}
};