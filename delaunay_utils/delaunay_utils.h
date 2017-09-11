#pragma once  

#include <Windows.h>
#include "common.h"
#include <thread>
#include "delaunay_utils.h"
#include "Utils.h"
#include "Vertex.h"

//#define DELAUNAYLIBRARY_API __declspec(dllexport) __stdcall

#define DllImport   extern "C" __declspec( dllimport )
#define DllExport   extern "C" __declspec( dllexport )

//DllExport triangleptr delaunay_dc(point_t* input, int input_size, int &output_size,double& volume);
DllExport triangleptr delaunay_dc(point_t* input, int input_size, int &output_size);
DllExport double volume(triangleptr input, int input_size);

//#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
