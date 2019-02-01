#include <Obstacle.h>

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

Obstacle::Obstacle(Obstacle const &other)
{
	vertices = other.vertices;
	edges = other.edges;
	AA = other.AA;
	BB = other.BB;

	for (size_t i = 1; i < vertices.size(); i++)
	{
		edges[i - 1].p1 = &vertices[i - 1];
		edges[i - 1].p2 = &vertices[i];
	}
	edges.back().p1 = &vertices.back();
	edges.back().p2 = &vertices[0];
}

bool Obstacle::isInside(Point &p)
{
	bool oddNodes = false;
	for (Edge &edge : edges)
	{
		if (
			((edge.p1->y < p.y && edge.p2->y >= p.y) || (edge.p2->y < p.y && edge.p1->y >= p.y)) &&
			(edge.p1->x <= p.x || edge.p2->x <= p.x))
		{
			if (edge.p1->x + (p.y - edge.p1->y) / (edge.p2->y - edge.p1->y) * (edge.p2->x - edge.p1->x) < p.x)
			{
				oddNodes = !oddNodes;
			}
		}
	}

	return oddNodes;
}

void from_json(const json &j, Obstacle &obstacle)
{
	obstacle.vertices = j.at("vertices").get<std::vector<Point>>();

	obstacle.AA.x = std::numeric_limits<float>::max();
	obstacle.AA.y = std::numeric_limits<float>::max();
	obstacle.BB.x = std::numeric_limits<float>::min();
	obstacle.BB.y = std::numeric_limits<float>::min();

	for (size_t i = 0; i < obstacle.vertices.size(); i++)
	{
		Point &v = obstacle.vertices[i];

		obstacle.AA.x = std::min(obstacle.AA.x, v.x);
		obstacle.AA.y = std::min(obstacle.AA.y, v.y);
		obstacle.BB.x = std::max(obstacle.BB.x, v.x);
		obstacle.BB.y = std::max(obstacle.BB.y, v.y);
		
		if (i > 0)
		{
			obstacle.edges.emplace_back(&obstacle.vertices[i - 1], &v);
		}
	}
	obstacle.edges.emplace_back(&obstacle.vertices.back(), &obstacle.vertices[0]);
}
