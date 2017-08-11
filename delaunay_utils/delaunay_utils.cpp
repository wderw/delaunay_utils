#include "delaunay_utils.h"

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

	return result.size();
}