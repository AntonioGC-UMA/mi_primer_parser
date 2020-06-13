#pragma once

#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono_literals;


struct timer
{
	chrono::time_point<chrono::high_resolution_clock> start_time;
	string name;

	timer(string name);
	~timer();
};

