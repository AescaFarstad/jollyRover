#include <Obstacle.h>

Obstacle::Obstacle(const Obstacle& other)
{
	vertices = other.vertices;
	extendedVertices[0] = other.extendedVertices[0];
	extendedVertices[1] = other.extendedVertices[1];
	extendedVertices[1] = other.extendedVertices[1];
	edges = other.edges;
	AA = other.AA;
	BB = other.BB;
	centroid = other.centroid;
	id = other.id;

	//The important bit:
	for (size_t i = 1; i < vertices.size(); i++)
	{
		edges[i - 1].p1 = &vertices[i - 1];
		edges[i - 1].p2 = &vertices[i];
	}
	edges.back().p1 = &vertices.back();
	edges.back().p2 = &vertices[0];
	
	for (size_t i = 0; i < vertices.size(); i++)
	{
		int32_t next = (i + 1) % vertices.size();		
		extendedEdges.emplace_back(&extendedVertices[0][i], &extendedVertices[1][next]);
	}
	//end of the important bit
}

bool Obstacle::isInside(Point& p)
{
	bool oddNodes = false;
	for (Edge& edge : edges)
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

const Point& Obstacle::getAA()
{
	return AA;
}

const Point& Obstacle::getBB()
{	
	return BB;
}
	
const Point& Obstacle::getCenter()
{	
	return centroid;
}

bool Obstacle::hitTest(Point& p)
{
	return isInside(p);
}

bool Obstacle::hitTest(Edge& e)
{
	if (isInside(*e.p1) || isInside(*e.p2))
		return true;
	for(auto& edge : edges)
	{
		if (!FMath::getEdgeIntersection(edge, e).isNaN())
			return true;
	}
	return false;
}
	
void from_json(const nlohmann::json& j, Obstacle& obstacle)
{
	obstacle.id = j.at("id");
	obstacle.vertices = j.at("vertices").get<std::vector<Point>>();
	

	obstacle.AA.x = std::numeric_limits<float>::max();
	obstacle.AA.y = std::numeric_limits<float>::max();
	obstacle.BB.x = std::numeric_limits<float>::min();
	obstacle.BB.y = std::numeric_limits<float>::min();
	
	
	float area = 0;
	float tmp;
	obstacle.centroid.x = 0;
	obstacle.centroid.y = 0;

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
		Point &vNext = obstacle.vertices[(i + 1) % obstacle.vertices.size()];
		
		//https://en.wikipedia.org/wiki/Centroid#Of_a_polygon
		tmp = v.x * vNext.y - vNext.x * v.y;
		area += tmp;
		obstacle.centroid.x += (v.x + vNext.x) * tmp;
		obstacle.centroid.y += (v.y + vNext.y) * tmp;
	}
	area /= 2;
	obstacle.centroid.x /= 6 * area;
	obstacle.centroid.y /= 6 * area;
	
	obstacle.edges.emplace_back(&obstacle.vertices.back(), &obstacle.vertices[0]);
	
	float extendSize = 2;
	for (size_t i = 0; i < obstacle.vertices.size(); i++)
	{
		int32_t prev = (i - 1 + obstacle.vertices.size()) % obstacle.vertices.size();
		int32_t next = (i + 1) % obstacle.vertices.size();
		
		auto& v1 = obstacle.vertices[prev];
		auto& v2 = obstacle.vertices[i];
		auto& v3 = obstacle.vertices[next];
		
		Point vec1 = v2 - v3;
		Point vec2 = v2 - v1;
		
		vec1.scaleTo(extendSize);
		vec2.scaleTo(extendSize);
		
		obstacle.extendedVertices[0].push_back(v2 + vec1);
		obstacle.extendedVertices[1].push_back(v2 + vec2);
		
		Point cTo1 = obstacle.extendedVertices[0].back() - obstacle.centroid;
		Point cTo2 = obstacle.extendedVertices[1].back() - obstacle.centroid;
		if (cTo1.crossProduct(cTo2) < 0)
			THROW_FATAL_ERROR("Incorrect obstacle");		
	}
	
	//Extended edges
	for (size_t i = 0; i < obstacle.vertices.size(); i++)
	{
		int32_t next = (i + 1) % obstacle.vertices.size();
		
		//extended edge is the same edge but longer
		obstacle.extendedEdges.emplace_back(&obstacle.extendedVertices[0][i], &obstacle.extendedVertices[1][next]);
	}
}
