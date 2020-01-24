#include <Edge.h>

Edge::Edge(Point* p1, Point* p2)
{
	this->p1 = p1;
	this->p2 = p2;
}

std::string Edge::toString()
{
	return p1->toString() + p2->toString();
}
