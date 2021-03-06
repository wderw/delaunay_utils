#pragma once

#include <Windows.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define PARALLEL_THRESHOLD 1000

// narzedzia
class Utils;

// klasy implementujace renderable:
struct point_t;
struct triangle_t;
class Vertex;
class Edge;
class Triangle;
class Vector;

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
