#include <iostream>
#include <cstdlib>

#include <Windows.h>
#include <ctime>
#include <iostream>
#include <fstream>

#define DELAUNAYLIBRARY_API __declspec(dllimport) __stdcall

struct point_t
{
	double x, y, z;
};

struct triangle_t
{
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
};

typedef triangle_t* triangleptr;

// delegaty do funkcji
typedef triangle_t*(*pDelaunayDC)(point_t*, int, int&,double&);

int main()
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"delaunay_utils.dll");

	if (!hGetProcIDDLL) 
	{
		std::cout << "could not load the dynamic library" << std::endl;
		return EXIT_FAILURE;
	}

	auto delaunay_dc = (pDelaunayDC)GetProcAddress(hGetProcIDDLL, "delaunay_dc");
	if (!delaunay_dc)
	{
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}

	
	point_t* points = new point_t[112];
	/*
	for (int i = 0; i < 10000; ++i) 
	{
		double quakex = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakey = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakez = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		points[i].x = quakex + rand() % 1000;
		points[i].y = quakey + rand() % 1000;
		points[i].z = 1.0;// quakez + rand() % 1000;
	}
	*/
	srand(time(NULL));

	std::ifstream file;
	file.open("delaunay.xyz", std::ios::in);
	for (int i = 0; i < 112; i++)
	{
		file >> points[i].x;
		file >> points[i].y;
		file >> points[i].z;
	}
		/*
		double quakex = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakey = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakez = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

		points[i].x += quakex;
		points[i].y += quakey;
		points[i].z += quakez;
	}
	*/
	
	clock_t begin = clock();
	
	int resultsize = -1;
	double volume = 6.66;
	triangle_t* result = delaunay_dc(points, 112, resultsize, volume);

	std::cout << "resultsize: " << resultsize << std::endl;
	std::cout << "volume: " << volume << std::endl;

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "elapsed time: " << elapsed_secs << std::endl;


	std::ofstream myfile;
	myfile.open("triangles.txt", std::ios::app);

	for (int i = 0; i < resultsize; i++)
	{
		myfile << result[i].x1 << std::endl;
		myfile << result[i].y1 << std::endl;
		myfile << result[i].z1 << std::endl;

		myfile << result[i].x2 << std::endl;
		myfile << result[i].y2 << std::endl;
		myfile << result[i].z2 << std::endl;

		myfile << result[i].x3 << std::endl;
		myfile << result[i].y3 << std::endl;
		myfile << result[i].z3 << std::endl;
	}
	myfile.close();

	system("pause");
	return 0;
}
