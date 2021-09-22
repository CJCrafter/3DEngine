﻿#pragma once

#include <vector>
#include <DirectXMath.h>

#include "Vertex.h"

class IndexedTriangleList
{
public:
	IndexedTriangleList() = default;
	IndexedTriangleList(std::vector<Vertex> verts_in, std::vector<unsigned short> indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}
	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& vertex : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.data);
			DirectX::XMStoreFloat3(
				&vertex.data,
				DirectX::XMVector3Transform(pos, matrix)
			);
		}
	}

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};
