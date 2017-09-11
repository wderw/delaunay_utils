#include "delaunay_utils.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>

/*
DllExport triangleptr delaunay_dc(point_t* input, int input_size, int &output_size, double& volume)
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
	Utils::recursiveDepth = pointset.size() < PARALLEL_THRESHOLD ? 0 : Utils::recursiveDepth;
	Utils::dt_dewall(pointset, AFL, 0);
	
	triangle_t* output = new triangle_t[Triangle::triangles.size()];

	// return the output size
	output_size = Triangle::triangles.size();

	// wypisuj wartosci wierzcholkow i indeksow
	Triangle* t = nullptr;
	Vertex* vertexMinZ = nullptr;
	double totalVolume = 0.0;

	// policz objetosc wzgledem plaszczyzny z = zmin
	double minZ = DBL_MAX;
	for (int i = 0; i < pointset.size(); ++i)
	{
		vertexMinZ = pointset[i];
		if (vertexMinZ->z < minZ)
		{
			minZ = vertexMinZ->z;
		}
	}

	for (int i = 0; i < Triangle::triangles.size(); ++i)
	{
		t = Triangle::triangles[i];

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
		//z1 = output[i].z1;
		z1 = output[i].z1 - minZ;

		x2 = output[i].x2;
		y2 = output[i].y2;
		//z2 = output[i].z2;
		z2 = output[i].z2 - minZ;

		x3 = output[i].x3;
		y3 = output[i].y3;
		//z3 = output[i].z3;
		z3 = output[i].z3 - minZ;		

		totalVolume += (z1 + z2 + z3)*(abs(x1*y2 - x2*y1 + x2*y3 - x3*y2 + x3*y1 - x1*y3)) / 6;		

		output[i].i1 = t->e0->v2->index;
		output[i].i2 = t->e1->v1->index;
		output[i].i3 = t->e2->v1->index;		
	}
	volume = totalVolume;	

	// wyczysc pamiec
	for (int i = 0; i < Triangle::triangles.size(); i++)
	{
		delete Triangle::triangles[i];
	}
	Triangle::triangles.clear();	

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
*/
DllExport double volume(triangleptr input, int input_size)
{
	// wypisuj wartosci wierzcholkow i indeksow
	triangleptr t = nullptr;
	triangleptr triangleMinZ = nullptr;
	double totalVolume = 0.0;

	// policz objetosc wzgledem plaszczyzny z = zmin
	double minZ = DBL_MAX;

	for (int i = 0; i < input_size; ++i)
	{
		triangleMinZ = &input[i];

		printf("z1=%lf\n", triangleMinZ->z1);
		printf("z2=%lf\n", triangleMinZ->z2);
		printf("z3=%lf\n", triangleMinZ->z3);
		
		if (triangleMinZ->z1 < minZ)
		{
			minZ = triangleMinZ->z1;
		}

		if (triangleMinZ->z2 < minZ)
		{
			minZ = triangleMinZ->z2;
		}

		if (triangleMinZ->z3 < minZ)
		{
			minZ = triangleMinZ->z3;
		}
	}
	printf("minZ=%lf\n", minZ);

	for (int i = 0; i < input_size; ++i)
	{
		t = &input[i];

		double x1, x2, x3, y1, y2, y3, z1, z2, z3;

		x1 = t->x1;
		y1 = t->y1;
		z1 = t->z1 - minZ;

		x2 = t->x2;
		y2 = t->y2;
		z2 = t->z2 - minZ;

		x3 = t->x3;
		y3 = t->y3;
		z3 = t->z3 - minZ;

		totalVolume += (z1 + z2 + z3)*(abs(x1*y2 - x2*y1 + x2*y3 - x3*y2 + x3*y1 - x1*y3)) / 6;
	}
	return totalVolume;
}

DllExport triangleptr delaunay_dc(point_t* input, int input_size, int &output_size)
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
	Utils::recursiveDepth = pointset.size() < PARALLEL_THRESHOLD ? 0 : Utils::recursiveDepth;
	Utils::dt_dewall(pointset, AFL, 0);

	triangle_t* output = new triangle_t[Triangle::triangles.size()];

	// return the output size
	output_size = Triangle::triangles.size();

	// wypisuj wartosci wierzcholkow i indeksow
	Triangle* t = nullptr;

	for (int i = 0; i < Triangle::triangles.size(); ++i)
	{
		t = Triangle::triangles[i];

		output[i].x1 = t->e0->v2->position.x;
		output[i].y1 = t->e0->v2->position.y;
		output[i].z1 = t->e0->v2->z;

		output[i].x2 = t->e1->v1->position.x;
		output[i].y2 = t->e1->v1->position.y;
		output[i].z2 = t->e1->v1->z;

		output[i].x3 = t->e2->v1->position.x;
		output[i].y3 = t->e2->v1->position.y;
		output[i].z3 = t->e2->v1->z;

		output[i].i1 = t->e0->v2->index;
		output[i].i2 = t->e1->v1->index;
		output[i].i3 = t->e2->v1->index;
	}

	// wyczysc pamiec
	for (int i = 0; i < Triangle::triangles.size(); i++)
	{
		delete Triangle::triangles[i];
	}
	Triangle::triangles.clear();

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
