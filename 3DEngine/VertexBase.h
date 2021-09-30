#pragma once

#include <DirectXMath.h>
#include "Vec3.h"

struct VertexBase
{
	union {
		DirectX::XMFLOAT3 pos;
		Vec3f vector;
	};

	VertexBase(const float x, const float y, const float z)
		: pos(x, y, z)
	{
	}

	VertexBase(const Vec3f& vector)
		: vector(vector)
	{
	}

	VertexBase(const VertexBase& vector)
		: vector(Vec3f(vector.vector))
	{
	}

	~VertexBase()
	{
	}
};
