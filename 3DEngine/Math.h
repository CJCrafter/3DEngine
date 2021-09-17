#pragma once

#include <cmath>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;
constexpr float TAU = 6.28318530f;
constexpr double TAU_D = 6.28318530717958647692;
constexpr float GOLDEN_ANGLE = 2.39996322f;
constexpr double GOLDEN_ANGLE_D = 2.39996322972865332;

template<typename T>
constexpr T Square(const T& num)
{
	return num * num;
}

template<typename T>
T NormalizeAngle(T theta)
{
	const T modded = fmod(theta, static_cast<T>(TAU_D));
	return (modded > PI_D ? modded - TAU_D : modded);
}

template<typename T>
constexpr T Interpolate(const T& min, const T& max, float factor)
{
	return min + factor * (max - min);
}

template<typename T>
constexpr T Radians(T degrees)
{
	return degrees * PI / 180.0f;
}