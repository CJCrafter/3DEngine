#pragma once
#include "IndexedTriangleList.h"
#include "VertexBase.h"

template<class T>
class Shape
{
	static_assert(std::is_base_of_v<VertexBase, T>, L"Vertex template MUST inherit from VertexBase");

public:
	const IndexedTriangleList<T>& Geometry() const { return geometry; }

protected:
	IndexedTriangleList<T> geometry;
};
