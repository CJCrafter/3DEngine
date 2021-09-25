#pragma once
#include "Shape.h"
#include <unordered_map>

class IcoSphere : public Shape
{
public:
	IcoSphere(int recursion);
private:
	unsigned int AddVertex(const Vec3f& point);
	unsigned int GetMiddlePoint(unsigned int indexA, unsigned int indexB);

private:
	std::unordered_map<unsigned long, unsigned int> cache;
	unsigned int index;
};