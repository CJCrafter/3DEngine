#pragma once
#include "Shape.h"
#include <unordered_map>

template<class T>
class IcoSphere : public Shape<T>
{
public:
	explicit IcoSphere(const int recursion)
		: index(0)
	{
		const float x = (1.0f + std::sqrt(5.0f)) / 2.0f;

		AddVertex(T{ -1,  x, 0 });
		AddVertex(T{ 1,  x, 0 });
		AddVertex(T{ -1, -x, 0 });
		AddVertex(T{ 1, -x, 0 });

		AddVertex(T{ 0, -1,  x });
		AddVertex(T{ 0,  1,  x });
		AddVertex(T{ 0, -1, -x });
		AddVertex(T{ 0,  1, -x });

		AddVertex(T{ x, 0, -1 });
		AddVertex(T{ x, 0,  1 });
		AddVertex(T{ -x, 0, -1 });
		AddVertex(T{ -x, 0,  1 });

		Shape<T>::geometry.indices =
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
			auto faces = std::vector<unsigned int>();
			faces.reserve(Shape<T>::geometry.indices.size() / 3 * 4);
			for (int iteration = 0; iteration < Shape<T>::geometry.indices.size(); iteration += 3)
			{
				const unsigned __int32 i = Shape<T>::geometry.indices[iteration];
				const unsigned __int32 j = Shape<T>::geometry.indices[1 + iteration];
				const unsigned __int32 k = Shape<T>::geometry.indices[2 + iteration];

				const unsigned __int32 a = GetMiddlePoint(i, j);
				const unsigned __int32 b = GetMiddlePoint(j, k);
				const unsigned __int32 c = GetMiddlePoint(k, i);

				faces.push_back(i); faces.push_back(a); faces.push_back(c);
				faces.push_back(j); faces.push_back(b); faces.push_back(a);
				faces.push_back(k); faces.push_back(c); faces.push_back(b);
				faces.push_back(a); faces.push_back(b); faces.push_back(c);
			}
			Shape<T>::geometry.indices = faces;
		}
	}

private:
	unsigned __int32 AddVertex(const T& point)
	{

		// We normalize a new point (not effecting  the passed parameter) so the
		// point will lay perfectly on a unit sphere (radius = 1).
		Shape<T>::geometry.vertices.push_back(T{ point.vector.GetNormalized() });
		return index++;
	}

	unsigned __int32 GetMiddlePoint(const unsigned __int32 indexA, const unsigned __int32 indexB)
	{
		// The cache stores a point as lower << 32 | upper
		// It is crucial to figure out if indexA is lower, or indexB is lower
		const unsigned __int64 lower = indexA < indexB ? indexA : indexB;
		const unsigned __int64 upper = indexA < indexB ? indexB : indexA;
		const unsigned __int64 hash = lower << 32 | upper;

		// Whenever find an old hash value, we can return a previous vertex instead
		// of creating a new one.
		if (cache.find(hash) != cache.end())
		{
			return cache[hash];
		}

		// Find the midpoint between the vectors
		const T& vertexA = Shape<T>::geometry.vertices[indexA];
		const T& vertexB = Shape<T>::geometry.vertices[indexB];
		const T& middle = (vertexA.vector + vertexB.vector) /= 2.0f;

		if (middle.vector.IsEmpty())
		{
			throw EngineException(__LINE__, __FILE__, "", "");
		}

		const unsigned __int32 i = AddVertex(middle);

		cache[hash] = i;
		return i;
	}

private:
	std::unordered_map<unsigned __int64, unsigned __int32> cache;
	unsigned __int32 index;
};