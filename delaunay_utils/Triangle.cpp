#include "common.h"
#include "Utils.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"

std::mutex Triangle::mutex;
std::vector<Triangle*> Triangle::triangles;

Triangle::Triangle()
{
	e0 = nullptr;
	e1 = nullptr;
	e2 = nullptr;

	Triangle::mutex.lock();
	Triangle::triangles.push_back(this);
	Triangle::mutex.unlock();
}

Triangle::Triangle(Edge *e0, Edge *e1, Edge *e2) : Triangle()
{
	this->e0 = e0;
	this->e1 = e1;
	this->e2 = e2;
}