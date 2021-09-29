#pragma once

#include "Shape.h"
#include "Math.h"

template<class T>
class PointSphere : public Shape<T>
{
public:
	explicit PointSphere(const int points)
	{
		std::vector<T> vertices;
		std::vector<unsigned int> indices;
		vertices.reserve(points);

		for (int i = 0; i < points; i++)
		{
			const float y = 1.0f - (static_cast<float>(i) / (static_cast<float>(points) - 1.0f)) * 2.0f;
			const float r = std::sqrt(1.0f - y * y);

			const float theta = static_cast<float>(i) * GOLDEN_ANGLE;

			const float x = r * std::cos(theta);
			const float z = r * std::sin(theta);
			vertices.push_back({ x / 2.0f, y / 2.0f, z / 2.0f });
		}

		Shape<T>::geometry = { vertices, indices };
	}

};