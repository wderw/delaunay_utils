#include "Vertex.h"
#include "Edge.h"

Edge::Edge()
{
	v1 = nullptr;
	v2 = nullptr;
}

Edge::Edge(Vertex * v1, Vertex * v2) : Edge()
{
	this->v1 = v1;
	this->v2 = v2;
}

void Edge::setOrigin(Vector origin)
{
	simplex_origin = origin;
}
