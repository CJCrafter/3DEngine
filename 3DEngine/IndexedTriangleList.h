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

	void SetNormals()
	{
		using namespace DirectX;
		assert(indices.size() % 3 == 0 && indices.size() > 0);
		for (size_t i = 0; i < indices.size(); i+= 3)
		{
			// v = vertex, p = point
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			const auto normal = XMVector3Normalize(XMVector3Cross(p1 - p0, p2 - p0));

			XMStoreFloat3(&v0.normal, normal);
			XMStoreFloat3(&v1.normal, normal);
			XMStoreFloat3(&v2.normal, normal);
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned int> indices;
};
