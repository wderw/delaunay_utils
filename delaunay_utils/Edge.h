#pragma once

#include "common.h"

class Edge
{
public:
	static std::vector<Edge*> allEdges;
	static std::mutex mutex;

	Vertex *v1, *v2;

	Vector simplex_origin;

	Edge();
	Edge(Vertex *v1, Vertex *v2);

	void setOrigin(Vector);
};