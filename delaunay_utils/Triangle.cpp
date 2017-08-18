#include "common.h"
#include "Utils.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"

Triangle::Triangle()
{
	e0 = nullptr;
	e1 = nullptr;
	e2 = nullptr;

	IRenderable::mutex.lock();
	IRenderable::triangles.push_back(this);
	IRenderable::mutex.unlock();
}

Triangle::Triangle(Edge *e0, Edge *e1, Edge *e2) : Triangle()
{
	this->e0 = e0;
	this->e1 = e1;
	this->e2 = e2;
}