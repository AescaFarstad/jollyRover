#pragma once
#include <string>
#include <SerializationStream.h>
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
		SerializationStream s = SerializationStream::createExp();
		target.serialize(s);
		CRC32  digestCrc32;
		char* data = s.readAll();
		digestCrc32.add(data, s.getLength());
		delete[] data;
		return digestCrc32.getHash();		
	}
}