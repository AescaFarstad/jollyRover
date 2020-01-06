#pragma once
#include <cstdint>
class SystemInfo
{
public:
	static bool isBigEndian;

private:

	static bool testIsBigEndian()
	{
		int16_t test = 1;
		return  (*((char*)&test)) == 0;
	}
};