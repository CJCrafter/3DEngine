#include "Cylinder.h"
#include "Math.h"

#define WRAP(index) ((index) >= divisions + 2) ? ((index) % (divisions + 2) + 2) : (index)

Cylinder::Cylinder(int divisions)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	vertices.reserve(2 + divisions * 2);
	indices.reserve(divisions * 12);

	vertices.push_back(Vertex(Vec3f(0.0f,  1.0f, 0.0f)));
	vertices.push_back(Vertex(Vec3f(0.0f, -1.0f, 0.0f)));

	const float delta = TAU / static_cast<float>(divisions);
	float theta = 0.0f;
	divisions *= 2;
	for (int i = 0; i < divisions; i += 2)
	{
		const float x = cos(theta);
		const float z = sin(theta);

		vertices.push_back(Vertex(Vec3f(x,  1.0f, z)));
		vertices.push_back(Vertex(Vec3f(x, -1.0f, z)));

		// Connect to the top face
		indices.push_back(WRAP(i + 4));
		indices.push_back(WRAP(i + 2));
		indices.push_back(WRAP(0));

		// Connect to the bottom face
		indices.push_back(WRAP(i + 3));
		indices.push_back(WRAP(i + 5));
		indices.push_back(WRAP(1));

		// Connect the sides
		indices.push_back(WRAP(i + 4));
		indices.push_back(WRAP(i + 3));
		indices.push_back(WRAP(i + 2));

		indices.push_back(WRAP(i + 4));
		indices.push_back(WRAP(i + 5));
		indices.push_back(WRAP(i + 3));
		

		theta += delta;
	}

	geometry = { vertices, indices };
}