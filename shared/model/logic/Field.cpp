#include <Field.h>

namespace Field
{
	Obstacle* findObstacle(Point& loc, Prototypes* prototypes)
	{
		auto result = std::find_if(prototypes->obstacles.begin(), prototypes->obstacles.end(), [&loc](Obstacle& obstacle){
			return obstacle.isInside(loc);
		});
		return result == prototypes->obstacles.end() ? nullptr : &*result;
	}
}