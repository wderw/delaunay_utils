#include <iostream>
#include <cstdlib>

#include <Windows.h>


#define DELAUNAYLIBRARY_API __declspec(dllimport) __stdcall

// delegaty do funkcji
typedef double(*functionAdd)(double A, double B);


int main()
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"delaunay_utils.dll");

	if (!hGetProcIDDLL) 
	{
		std::cout << "could not load the dynamic library" << std::endl;
		return EXIT_FAILURE;
	}

	auto Add = (functionAdd)GetProcAddress(hGetProcIDDLL, "Add");
	if (!Add) 
	{
		std::cout << "could not locate the function" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << Add(5, 3);

	system("pause");
	return 0;
}
