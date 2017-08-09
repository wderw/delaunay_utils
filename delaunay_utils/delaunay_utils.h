#pragma once  

#include <Windows.h>

#define DELAUNAYLIBRARY_API __declspec(dllexport) __stdcall

extern "C"
{
	double DELAUNAYLIBRARY_API Add(double a, double b);
}

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
