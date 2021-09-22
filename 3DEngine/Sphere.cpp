#include "Sphere.h"
#include "Math.h"

Sphere::Sphere(const int points)
	: points(points)
{
}

IndexedTriangleList Sphere::Generate() const
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
		const float z = r * std::cos(theta);
		Vertex vertex(Vec3f(x, y, z));
		vertices.push_back(vertex);
	}

	return {vertices, indices};
}
