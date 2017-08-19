#pragma once

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
	int i1, i2, i3;
};

typedef triangle_t* triangleptr;

// delegaty do funkcji
typedef triangle_t*(*pDelaunayDC)(point_t*, int, int&, double&); 
