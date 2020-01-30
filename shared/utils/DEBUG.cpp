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
#endif