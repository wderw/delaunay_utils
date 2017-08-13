#pragma once

#include "common.h"
#include "IRenderable.h"
#include "Vector.h"

class Vertex
{
public:
	static std::vector<Vertex*> vertices;

	Vector position;
	double z;

	Vertex();
	Vertex(double, double, double);

	void moveTo(double, double);
	void moveTo(Vector);
	void moveBy(double, double);
};