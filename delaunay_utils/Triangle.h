#pragma once

#include "common.h"
#include "Edge.h"

class Triangle
{
public:
	Edge* e0;
	Edge* e1;
	Edge* e2;

	Triangle();
	Triangle(Edge*, Edge*, Edge*);
};