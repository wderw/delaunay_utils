#pragma once

#include "common.h"
#include "Vector.h"

class Vertex
{
public:
	static std::vector<Vertex*> vertices;

	// pozycja na plaszczyznie w postaci wektora 2d
	// i jego uzupelnienia w postaci wysokosci (z)
	Vector position;
	double z;

	// indeks wierzcholka w wejsciowej tablicy danych
	int index;

	Vertex();
	Vertex(double, double, double, int);

	void moveTo(double, double);
	void moveTo(Vector);
	void moveBy(double, double);
};