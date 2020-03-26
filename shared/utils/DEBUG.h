#pragma once
#include <string>
#include <BinarySerializer.h>
#include <JSONSerializer.h>

#include <crc32.h>
#ifndef __EMSCRIPTEN__

	//#include <glob.h>
	#include <iostream>
	#include <fstream>

	void traceFiles(std::string path);
	void dump(std::string data, std::string name = "dump");	
	template <typename T>
	void dump(const T& object, const std::string name)
	{
		JSONSerializer j;
		j.write(object);
		dump(j.toString(), name);
	}
	
	void writeBinary(std::vector<char> data, std::string name);
	std::vector<char> readBinary(std::string name);

#endif
