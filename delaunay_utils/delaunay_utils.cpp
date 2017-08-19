#include "delaunay_utils.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>

triangleptr DELAUNAYLIBRARY_API delaunay_dc(point_t* input, int input_size, int &output_size, double& volume)
{
	std::vector<Vertex*> pointset;
	std::list<Edge*> AFL;

	int i;
	for (i = 0; i < input_size; ++i)
	{
		Vertex* vertex = new Vertex(input[i].x, input[i].y, input[i].z, i);
		pointset.push_back(vertex);
	}

	std::vector<triangle_t*> result;

	Utils::dt_dewall(pointset, AFL, 0);
	
	triangle_t* output = new triangle_t[IRenderable::triangles.size()];

	// return the output size
	output_size = IRenderable::triangles.size();

	/* wypisuj wartosci wierzcholkow i indeksow */
	Triangle* t = nullptr;
	double totalVolume = 0.0;

	for (int i = 0; i < IRenderable::triangles.size(); i++)
	{
		t = IRenderable::triangles[i];

		output[i].x1 = t->e0->v2->position.x;
		output[i].y1 = t->e0->v2->position.y;
		output[i].z1 = t->e0->v2->z;

		output[i].x2 = t->e1->v1->position.x;
		output[i].y2 = t->e1->v1->position.y;
		output[i].z2 = t->e1->v1->z;

		output[i].x3 = t->e2->v1->position.x;
		output[i].y3 = t->e2->v1->position.y;
		output[i].z3 = t->e2->v1->z;

		double x1, x2, x3, y1, y2, y3, z1, z2, z3;

		x1 = output[i].x1;
		y1 = output[i].y1;
		z1 = output[i].z1;

		x2 = output[i].x2;
		y2 = output[i].y2;
		z2 = output[i].z2;

		x3 = output[i].x3;
		y3 = output[i].y3;
		z3 = output[i].z3;

		totalVolume += (z1 + z2 + z3)*(abs(x1*y2 - x2*y1 + x2*y3 - x3*y2 + x3*y1 - x1*y3)) / 6;
		

		output[i].i1 = t->e0->v2->index;
		output[i].i2 = t->e1->v1->index;
		output[i].i3 = t->e2->v1->index;		
	}
	volume = totalVolume;
	

	/* wyczysc pamiec */
	for (int i = 0; i < IRenderable::triangles.size(); i++)
	{
		delete IRenderable::triangles[i];
	}
	IRenderable::triangles.clear();
	

	for (int i = 0; i < Edge::allEdges.size(); i++)
	{
		delete Edge::allEdges[i];
	}
	Edge::allEdges.clear();


	for (int i = 0; i < pointset.size(); i++)
	{
		delete pointset[i];
	}
	pointset.clear();	

	return output;
}

