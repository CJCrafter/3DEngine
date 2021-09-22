#pragma once

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(int points);
	IndexedTriangleList Generate() const override;
private:
	int points;
};