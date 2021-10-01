 #pragma once

#include "Shape.h"

template<class T>
class SkinnedCube : public Shape<T>
{
public:
	SkinnedCube()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<T> vertices(14);

		// a
		vertices[0].pos = Vec3f(-side, side, side);
		vertices[0].texture = { 1.0f / 4.0f, 1.0f / 3.0f };

		// b
		vertices[1].pos = Vec3f(-side, -side, side);
		vertices[1].texture = { 1.0f / 4.0f, 2.0f / 3.0f };

		// c
		vertices[2].pos = Vec3f(side, -side, side);
		vertices[2].texture = { 2.0f / 4.0f, 2.0f / 3.0f };

		// d
		vertices[3].pos = Vec3f(side, side, side);
		vertices[3].texture = { 2.0f / 4.0f, 1.0f / 3.0f };

		// e 1-3
		vertices[4].pos = Vec3f(-side, side, -side);
		vertices[4].texture = { 0.0f / 4.0f, 1.0f / 3.0f };
		vertices[5].pos = Vec3f(-side, side, -side);
		vertices[5].texture = { 1.0f / 4.0f, 0.0f / 3.0f };
		vertices[6].pos = Vec3f(-side, side, -side);
		vertices[6].texture = { 4.0f / 4.0f, 1.0f / 3.0f };

		// f 1-3
		vertices[7].pos = Vec3f(-side, -side, -side);
		vertices[7].texture = { 0.0f / 4.0f, 2.0f / 3.0f };
		vertices[8].pos = Vec3f(-side, -side, -side);
		vertices[8].texture = { 1.0f / 4.0f, 3.0f / 3.0f };
		vertices[9].pos = Vec3f(-side, -side, -side);
		vertices[9].texture = { 4.0f / 4.0f, 2.0f / 3.0f };

		// g 1-2
		vertices[10].pos = Vec3f(side, -side, -side);
		vertices[10].texture = { 2.0f / 4.0f, 3.0f / 3.0f };
		vertices[11].pos = Vec3f(side, -side, -side);
		vertices[11].texture = { 3.0f / 4.0f, 2.0f / 3.0f };

		// h 1-2
		vertices[12].pos = Vec3f(side, side, -side);
		vertices[12].texture = { 2.0f / 4.0f, 0.0f / 3.0f };
		vertices[13].pos = Vec3f(side, side, -side);
		vertices[13].texture = { 3.0f / 4.0f, 1.0f / 3.0f };

		Shape<T>::geometry = {
			std::move(vertices),{
				5,0,3,   12,5,3,
				4,7,1,   0,4,1,
				0,1,2,   3,0,2,
				3,2,11,  13,3,11,
				13,11,9, 6,13,9,
				1,8,10,  2,1,10
			}
		};
	}
};
