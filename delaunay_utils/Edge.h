#pragma once

#include "common.h"

class Edge
{
public:
	Vertex *v1, *v2;

	Vector simplex_origin;

	Edge();
	Edge(Vertex *v1, Vertex *v2);

	void setOrigin(Vector);
};