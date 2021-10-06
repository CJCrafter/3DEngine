#pragma once

#include "Shape.h"

template<class T>
class FacedCube : public Shape<T>
{
public:
	FacedCube()
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<T> vertices(24);
		vertices[0].pos  = { -side, -side, -side };
		vertices[1].pos  = { +side, -side, -side };
		vertices[2].pos  = { -side, +side, -side };
		vertices[3].pos  = { +side, +side, -side };
		vertices[4].pos  = { -side, -side, +side };
		vertices[5].pos  = { +side, -side, +side };
		vertices[6].pos  = { -side, +side, +side };
		vertices[7].pos  = { +side, +side, +side };
		vertices[8].pos  = { -side, -side, -side };
		vertices[9].pos  = { -side, +side, -side };
		vertices[10].pos = { -side, -side, +side };
		vertices[11].pos = { -side, +side, +side };
		vertices[12].pos = { +side, -side, -side };
		vertices[13].pos = { +side, +side, -side };
		vertices[14].pos = { +side, -side, +side };
		vertices[15].pos = { +side, +side, +side };
		vertices[16].pos = { -side, -side, -side };
		vertices[17].pos = { +side, -side, -side };
		vertices[18].pos = { -side, -side, +side };
		vertices[19].pos = { +side, -side, +side };
		vertices[20].pos = { -side, +side, -side };
		vertices[21].pos = { +side, +side, -side };
		vertices[22].pos = { -side, +side, +side };
		vertices[23].pos = { +side, +side, +side };

		Shape<T>::geometry = {
			std::move(vertices), {
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}
};
