#include "Vertex.h"
#include "Utils.h"

std::vector<Vertex*> Vertex::vertices;

Vertex::Vertex()
{
	Vertex::vertices.push_back(this);
}
Vertex::Vertex(double x, double y, double _z) : Vertex()
{
	position.x = x;
	position.y = y;
	z = _z;
}

inline void Vertex::moveTo(double x, double y)
{
	this->position.x = x;
	this->position.y = y;
}

inline void Vertex::moveTo(Vector newPos)
{
	this->position = newPos;
}
void Vertex::moveBy(double x, double y)
{
	this->position.x += x;
	this->position.y += y;
}
