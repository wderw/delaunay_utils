#pragma once  

#include <Windows.h>
#include "common.h"
#include <thread>
#include "delaunay_utils.h"
#include "Utils.h"
#include "Vertex.h"

#define DELAUNAYLIBRARY_API __declspec(dllexport) __stdcall

extern "C"
{
	triangleptr DELAUNAYLIBRARY_API delaunay_dc(point_t* input, int input_size, int &output_size,double& volume);
}


//#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
