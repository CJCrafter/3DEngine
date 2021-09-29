#pragma once

#include <vector>
#include <DirectXMath.h>

#include "VertexBase.h"

template<class T>
class IndexedTriangleList
{
	static_assert(std::is_base_of_v<VertexBase, T>, L"Vertex template MUST inherit from VertexBase");

public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<T> v, std::vector<unsigned int> i)
		:
		vertices(std::move(v)),
		indices(std::move(i))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}
	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& vertex : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.pos);
			DirectX::XMStoreFloat3(
				&vertex.pos,
				DirectX::XMVector3Transform(pos, matrix)
			);
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned int> indices;
};
