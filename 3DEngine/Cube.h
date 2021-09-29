#pragma once

#include "Shape.h"

template<class Vertex>
class Cube : public Shape<Vertex>
{
public:
	Cube()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<Vertex> vertices;
		vertices.emplace_back(Vertex{ Vec3f(-side, -side, -side) });
		vertices.emplace_back(Vertex{ Vec3f( side, -side, -side) });
		vertices.emplace_back(Vertex{ Vec3f(-side,  side, -side) });
		vertices.emplace_back(Vertex{ Vec3f( side,  side, -side) });
		vertices.emplace_back(Vertex{ Vec3f(-side, -side,  side) });
		vertices.emplace_back(Vertex{ Vec3f( side, -side,  side) });
		vertices.emplace_back(Vertex{ Vec3f(-side,  side,  side) });
		vertices.emplace_back(Vertex{ Vec3f( side,  side,  side) });

		Shape<Vertex>::geometry = {
			vertices,
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}
};
