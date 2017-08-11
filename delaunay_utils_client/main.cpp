#include <iostream>
#include <cstdlib>

#include <Windows.h>
#include <ctime>

#define DELAUNAYLIBRARY_API __declspec(dllimport) __stdcall

struct point_t
{
	double x, y, z;
};

struct triangle_t
{
	double x1, y1;
	double x2, y2;
	double x3, y3;

	triangle_t() {}
	triangle_t(double x1, double y1, double x2, double y2, double x3, double y3)
	{
		this->x1 = x1;
		this->y1 = y1;

		this->x2 = x2;
		this->y2 = y2;

		this->x3 = x3;
		this->y3 = y3;
	}
};

typedef triangle_t* triangleptr;


// delegaty do funkcji
typedef int(*pIntVoid)(void);
typedef int(*pIntPointptrInt)(point_t*, int);

int main()
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"delaunay_utils.dll");

	if (!hGetProcIDDLL) 
	{
		std::cout << "could not load the dynamic library" << std::endl;
		return EXIT_FAILURE;
	}

	auto get_cores_number = (pIntVoid)GetProcAddress(hGetProcIDDLL, "get_cores_number");
	if (!get_cores_number)
	{
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}

	auto delaunay_dc = (pIntPointptrInt)GetProcAddress(hGetProcIDDLL, "delaunay_dc");
	if (!delaunay_dc)
	{
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << get_cores_number() << std::endl;

	point_t* points = new point_t[1000];

	for (int i = 0; i < 1000; ++i) 
	{
		double quakex = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakey = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		double quakez = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		points[i].x = quakex + rand() % 1000;
		points[i].y = quakey + rand() % 1000;
		points[i].z = quakez + rand() % 1000;
	}	

	int result = delaunay_dc(points, 1000);
	std::cout << result << std::endl;

	system("pause");
	return 0;
}
