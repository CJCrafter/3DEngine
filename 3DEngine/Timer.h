#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
	float frequency;
	steady_clock::time_point lastTime;
	float period;

public:
	Timer(float frequency);
	
	bool Ready() const;
	void Mark();
	float Percentage() const;
	float Elapsed() const;
};
