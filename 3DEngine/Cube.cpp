#include "Cube.h"

Cube::Cube()
{
	constexpr float side = 1.0f / 2.0f;

	std::vector<Vertex> vertices;
	vertices.emplace_back(Vec3f{ -side, -side, -side }); // 0
	vertices.emplace_back(Vec3f{  side, -side, -side }); // 1
	vertices.emplace_back(Vec3f{ -side,  side, -side }); // 2
	vertices.emplace_back(Vec3f{  side,  side, -side }); // 3
	vertices.emplace_back(Vec3f{ -side, -side,  side }); // 4
	vertices.emplace_back(Vec3f{  side, -side,  side }); // 5
	vertices.emplace_back(Vec3f{ -side,  side,  side }); // 6
	vertices.emplace_back(Vec3f{  side,  side,  side }); // 7

	geometry = {
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
