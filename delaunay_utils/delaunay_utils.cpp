#include "delaunay_utils.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>

int DELAUNAYLIBRARY_API delaunay_dc(point_t* input, int input_size)
{
	std::vector<Vertex*> pointset;
	std::list<Edge*> AFL;

	int i;
	for (i = 0; i < input_size; ++i)
	{
		Vertex* vertex = new Vertex(input[i].x, input[i].y);
		pointset.push_back(vertex);
	}

	std::vector<triangle_t*> result;

	clock_t begin = clock();

	Utils::dt_dewall(pointset, AFL, 0);

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << "elapsed time: " << elapsed_secs << std::endl;

	printf("renderables: %d \n", IRenderable::renderables.size());

	std::ofstream myfile;
	myfile.open("triangles.txt", std::ios::app);

	for (int i = 0; i < IRenderable::renderables.size(); i++)
	{
		Triangle* t = IRenderable::renderables[i];
		myfile << t->e0->v1->position.x << std::endl;
		myfile << t->e0->v1->position.y << std::endl;

		myfile << t->e1->v1->position.x << std::endl;
		myfile << t->e1->v1->position.y << std::endl;

		myfile << t->e2->v1->position.x << std::endl;
		myfile << t->e2->v1->position.y << std::endl;
	}

	myfile.close();
	return result.size();
}