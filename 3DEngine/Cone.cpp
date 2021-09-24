#include "Cone.h"
#include "Math.h"

#define WRAP(index) ((index) >= divisions + 2) ? ((index) % (divisions + 2) + 2) : (index)

Cone::Cone(int divisions)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	vertices.reserve(2 + divisions);
	vertices.push_back(Vertex(Vec3f(0.0f, 1.0f, 0.0f)));
	vertices.push_back(Vertex(Vec3f(0.0f, -1.0f, 0.0f)));

	const float delta = TAU / static_cast<float>(divisions);
	for (int i = 0; i < divisions; i++)
	{
		const float theta = delta * i;
		const float x = cos(theta);
		const float z = sin(theta);

		vertices.push_back(Vertex(Vec3f(x, -1.0, z)));

		// These 3 indices make a triangle to the tip
		indices.push_back(WRAP(i + 3));
		indices.push_back(WRAP(i + 2));
		indices.push_back(0);

		// These 3 indices make a triangle to the bottom circle
		indices.push_back(1);
		indices.push_back(WRAP(i + 2));
		indices.push_back(WRAP(i + 3));
	}

	geometry = { vertices, indices };
}