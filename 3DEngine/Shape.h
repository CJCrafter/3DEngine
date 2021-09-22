#pragma once
#include "IndexedTriangleList.h"

struct Shape
{
	virtual IndexedTriangleList Generate() const = 0;
};
