#include "IcoSphere.h"
#include "Math.h"

IcoSphere::IcoSphere(const int recursion)
	: index(0)
{
	float x = (1.0f + std::sqrt(5.0f)) / 2.0f;

	AddVertex({ -1,  x, 0 });
	AddVertex({  1,  x, 0 });
	AddVertex({ -1, -x, 0 });
	AddVertex({  1, -x, 0 });

	AddVertex({ 0, -1,  x });
	AddVertex({ 0,  1,  x });
	AddVertex({ 0, -1, -x });
	AddVertex({ 0,  1, -x });

	AddVertex({  x, 0, -1 });
	AddVertex({  x, 0,  1 });
	AddVertex({ -x, 0, -1 });
	AddVertex({ -x, 0,  1 });

	geometry.indices = 
	{
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};

	for (int ignore = 0; ignore < recursion; ignore++) 
	{
		auto faces = std::vector<unsigned short>();
		faces.reserve(geometry.indices.size() / 3 * 4);
		for (int iteration = 0; iteration < geometry.indices.size(); iteration += 3)
		{
			const int i = geometry.indices[iteration];
			const int j = geometry.indices[1 + iteration];
			const int k = geometry.indices[2 + iteration];

			const unsigned short a = GetMiddlePoint(i, j);
			const unsigned short b = GetMiddlePoint(j, k);
			const unsigned short c = GetMiddlePoint(k, i);

			faces.push_back(i); faces.push_back(a); faces.push_back(c);
			faces.push_back(j); faces.push_back(b); faces.push_back(a);
			faces.push_back(k); faces.push_back(c); faces.push_back(b);
			faces.push_back(a); faces.push_back(b); faces.push_back(c);
		}
		geometry.indices = faces;
	}
}

unsigned short IcoSphere::AddVertex(const Vec3f& point)
{

	// We normalize a new point (not effecting  the passed parameter) so the
	// point will lay perfectly on a unit sphere (radius = 1).
	geometry.vertices.push_back(Vertex(point.GetNormalized()));
	return index++;
}

unsigned short IcoSphere::GetMiddlePoint(const unsigned short indexA, const unsigned short indexB)
{
	// The cache stores a point as lower << 32 | upper
	// It is crucial to figure out if indexA is lower, or indexB is lower
  	const unsigned short lower = indexA < indexB ? indexA : indexB;
	const unsigned short upper = indexA < indexB ? indexB : indexA;
	const unsigned int   hash = static_cast<unsigned int>(lower) << 16 | upper;

	// Whenever find an old hash value, we can return a previous vertex instead
	// of creating a new one.
	if (cache.find(hash) != cache.end())
	{
		return cache[hash];
	}

	// Find the midpoint between the vectors
	const Vertex& vertexA  = geometry.vertices[indexA];
	const Vertex& vertexB  = geometry.vertices[indexB];
	const Vec3f&  middle   = (vertexA.vector + vertexB.vector) /= 2.0f;

	if (middle.IsEmpty())
	{
		throw EngineException(__LINE__, __FILE__, "", "");
	}

	const unsigned short i = AddVertex(middle);

	cache[hash] = i;
	return i;
}
