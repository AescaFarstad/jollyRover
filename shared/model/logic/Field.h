#include <Prototypes.h>

namespace Field
{
	Obstacle* findObstacle(Point& loc, Prototypes* prototypes);
	Edge* nearestIntersectedEdge(Point& from, Point& to, std::vector<Edge>& testEdges, std::vector<Edge>& returnEdges);
	
}