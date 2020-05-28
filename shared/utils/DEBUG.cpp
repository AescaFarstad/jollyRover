#include <DEBUG.h>
#ifndef __EMSCRIPTEN__

	void traceFiles(std::string path)
	{/*
		glob_t glob_result;
		glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
		
		for(unsigned int32_t i = 0; i < glob_result.gl_pathc; ++i)
		{
			std::cout << glob_result.gl_pathv[i] << std::endl;
		}*/
	}
	
	void dump(const std::string data, const std::string name)
	{
		std::ofstream file("../dumps/" + name + ".txt");
		file << data;
		file.close();
	}
	
	void writeBinary(std::vector<char> data, std::string name = "dump")
	{
		auto file = std::fstream("../dumps/" + name + ".binary", std::ios::out | std::ios::binary);
		file.write(data.data(), data.size());
		file.close();
	}
	
	std::vector<char> readBinary(std::string name = "dump")
	{
		auto file = std::fstream("../dumps/" + name + ".binary", std::ios::in | std::ios::binary);
		std::vector<char> result (std::istreambuf_iterator<char>(file), {});
		file.close();
		return result;
	}
#endif