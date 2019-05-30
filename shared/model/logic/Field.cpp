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
	
	Edge* nearestIntersectedEdge(Point& from, Point& to, std::vector<Edge>& testEdges, std::vector<Edge>& returnEdges)
	{
		float bestDistance = FMath::F_MAX;					
		Edge testEdge(&from, &to);
		Edge* bestEdge = nullptr;
		
		for(size_t i = 0; i < testEdges.size(); i++)
		{
			Point intersection = FMath::getEdgeIntersection(testEdge, testEdges[i]);
			if (!intersection.isNaN())
			{
				float distance = from.distanceTo(intersection);
				if (distance < bestDistance)
				{
					bestDistance = distance;
					bestEdge = &returnEdges[i];
				}
			}
		}
		return bestEdge;
	}
}