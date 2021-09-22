#pragma once
#include <DirectXMath.h>

#include "Vec3.h"

class Vertex
{
public:
	union
	{
		DirectX::XMFLOAT3 data;
		Vec3f vector;
	};

	Vertex()
	{
	}
	
	explicit Vertex(const Vec3f& vector)
		: vector(vector)
	{
	}

	explicit Vertex(const DirectX::XMFLOAT3& data)
		: data(data)
	{
	}

	Vertex(const Vertex& other)
		: data(other.data)
	{
	}

	Vertex(Vertex&& other) noexcept
		: data(std::move(other.data))
	{
	}

	Vertex& operator=(const Vertex& other)
	{
		if (this == &other)
			return *this;
		data = other.data;
		return *this;
	}

	Vertex& operator=(Vertex&& other) noexcept
	{
		if (this == &other)
			return *this;
		data = std::move(other.data);
		return *this;
	}

	~Vertex()
	{
	}
};
