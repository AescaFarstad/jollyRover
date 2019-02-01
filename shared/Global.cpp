#include <Global.h>
namespace S
{
	Logger log;
	int32_t idCounter = 0;
	int32_t getId()
	{
		return idCounter++;
	}
}
