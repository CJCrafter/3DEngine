#pragma once

#include <vector>
#include <array>
#include "IndexedTriangleList.h"

class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int divisions_x, int divisions_y)
	{
		assert(divisions_x >= 1);
		assert(divisions_y >= 1);

		constexpr float width = 2.0f;
		constexpr float height = 2.0f;
		const int nVertices_x = divisions_x + 1;
		const int nVertices_y = divisions_y + 1;
		std::vector<V> vertices(nVertices_x * nVertices_y);

		{
			const float side_x = width / 2.0f;
			const float side_y = height / 2.0f;
			const float divisionSize_x = width / static_cast<float>(divisions_x);
			const float divisionSize_y = height / static_cast<float>(divisions_y);
			const auto bottomLeft = DirectX::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);

			for (int y = 0, i = 0; y < nVertices_y; y++)
			{
				const float y_pos = static_cast<float>(y) * divisionSize_y;
				for (int x = 0; x < nVertices_x; x++, i++)
				{
					const auto v = DirectX::XMVectorAdd(
						bottomLeft,
						DirectX::XMVectorSet(static_cast<float>(x) * divisionSize_x, y_pos, 0.0f, 0.0f)
					);
					DirectX::XMStoreFloat3(&vertices[i].pos, v);
				}
			}
		}

		std::vector<unsigned short> indices;
		indices.reserve(sq(divisions_x * divisions_y) * 6);
		{
			const auto vxy2i = [nVertices_x](size_t x, size_t y)
			{
				return static_cast<unsigned short>(y * nVertices_x + x);
			};
			for (size_t y = 0; y < divisions_y; y++)
			{
				for (size_t x = 0; x < divisions_x; x++)
				{
					const std::array<unsigned short, 4> indexArray =
					{ vxy2i(x,y),vxy2i(x + 1,y),vxy2i(x,y + 1),vxy2i(x + 1,y + 1) };
					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}
		}

		return{ std::move(vertices),std::move(indices) };
	}
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(1, 1);
	}
};