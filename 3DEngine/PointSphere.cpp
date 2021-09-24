#include "PointSphere.h"
#include "Math.h"

PointSphere::PointSphere(const int points)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	vertices.reserve(points);

	for (int i = 0; i < points; i++)
	{
		const float y = 1.0f - (static_cast<float>(i) / (static_cast<float>(points) - 1.0f)) * 2.0f;
		const float r = std::sqrt(1.0f - Square(y));

		const float theta = static_cast<float>(i) * GOLDEN_ANGLE;

		const float x = r * std::cos(theta);
		const float z = r * std::sin(theta);
		Vertex vertex(Vec3f(x / 2.0f, y / 2.0f, z / 2.0f));
		vertices.push_back(vertex);
	}

	geometry = { vertices, indices };
}
