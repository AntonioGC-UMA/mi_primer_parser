#include "timer.h"

timer::timer(string name) : name(name), start_time(chrono::high_resolution_clock::now())
{
	
}

timer::~timer()
{
	chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();

	double time = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count() / 1000000.0;

	printf("%s: %f seconds\n", name.c_str(), time);
}