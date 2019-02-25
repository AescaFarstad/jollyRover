#ifndef __EMSCRIPTEN__
	#include <DEBUG.h>

	void traceFiles(std::string path)
	{
		glob_t glob_result;
		glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
		
		for(unsigned int i = 0; i < glob_result.gl_pathc; ++i)
		{
			std::cout << glob_result.gl_pathv[i] << std::endl;
		}
	}
#endif

