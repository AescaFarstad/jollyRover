#include <Field.h>

namespace Field
{
	Obstacle* findObstacle(Point& loc, Prototypes* prototypes)
	{
		auto options = prototypes->obstacleMap.getInCell(loc);
		auto result = std::find_if(options.begin(), options.end(), [&loc](Obstacle*& obstacle){
			return obstacle->isInside(loc);
		});
		return result == options.end() ? nullptr : *result;
	}
}