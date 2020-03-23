#pragma once
#include <string>
#include <BinarySerializer.h>

#include <crc32.h>
#ifndef __EMSCRIPTEN__

	//#include <glob.h>
	#include <iostream>
	#include <fstream>

	void traceFiles(std::string path);
	void dump(std::string data, std::string name = "dump");

#endif
