#pragma once
#include "Shape.h"
#include <unordered_map>

class IcoSphere : public Shape
{
public:
	IcoSphere(int recursion);
private:
	unsigned short AddVertex(const Vec3f& point);
	unsigned short GetMiddlePoint(unsigned short indexA, unsigned short indexB);

private:
	std::unordered_map<unsigned int, unsigned short> cache;
	unsigned short index;
};