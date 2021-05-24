#include "Timer.h"
#include <chrono>

using namespace std::chrono;

Timer::Timer(float frequency)
	:
	frequency(frequency),
	lastTime(steady_clock::now()),
	period(1.0f / frequency)
{}

bool Timer::Ready() const
{
	return Elapsed() > period;
}

void Timer::Mark()
{
	lastTime = steady_clock::now();
}

float Timer::Percentage() const
{
	return Elapsed() / period;
}

float Timer::Elapsed() const
{
	const auto now = steady_clock::now();
	const duration<float> elapsed = now - lastTime;

	return elapsed.count();
}