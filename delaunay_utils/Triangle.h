#pragma once

#include "common.h"
#include "Edge.h"
#include <memory>
#include <mutex>

class Triangle
{
public:
	static std::mutex mutex;
	static std::vector<Triangle*> triangles;

	Edge* e0;
	Edge* e1;
	Edge* e2;

	Triangle();
	Triangle(Edge*, Edge*, Edge*);
};