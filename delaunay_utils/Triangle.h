#pragma once

#include "common.h"
#include "IRenderable.h"
#include "Edge.h"
#include <memory>

class Triangle : public IRenderable
{
public:
	Edge* e0;
	Edge* e1;
	Edge* e2;

	Triangle();
	Triangle(Edge*, Edge*, Edge*);
};