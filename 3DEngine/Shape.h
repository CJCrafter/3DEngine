#pragma once
#include "IndexedTriangleList.h"

class Shape
{
public:
	IndexedTriangleList Geometry() const { return geometry; }

protected:
	IndexedTriangleList geometry;
};
