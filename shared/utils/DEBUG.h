#pragma once
#include <string>
#include <BinarySerializer.h>

#include <crc32.h>
#ifndef __EMSCRIPTEN__

	//#include <glob.h>
	#include <iostream>

	void traceFiles(std::string path);

#endif

namespace S 
{
	template <typename T>
	std::string crc(const T& target)
	{
		BinarySerializer bs;
		Serialization::write(target, bs);
		CRC32  digestCrc32;
		auto data = bs.dumpAll();
		digestCrc32.add(&data[0], data.size());
		return digestCrc32.getHash();		
	}
}