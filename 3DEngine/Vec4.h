#pragma once
#include <sstream>

#include "EngineException.h"

template<class T>
struct Vec4
{
	union
	{
		T data[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
	};

	Vec4()
		:
		x(0),
		y(0),
		z(0),
		w(0)
	{
	}

	Vec4(T x, T y, T z, T w)
		:
		x(x),
		y(y),
		z(z),
		w(w)
	{
	}

	Vec4(const Vec4& other)
		:
		x(other.x),
		y(other.y),
		z(other.z),
		w(other.w)
	{
	}

	Vec4(Vec4&& other) noexcept
		:
		x(std::move(other.x)),
		y(std::move(other.y)),
		z(std::move(other.z)),
		w(std::move(other.w))
	{
	}

	Vec4<T>& operator=(const Vec4<T>& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	Vec4<T>& operator=(Vec4<T>&& other) noexcept
	{
		if (this == &other)
			return *this;
		x = std::move(other.x);
		y = std::move(other.y);
		z = std::move(other.z);
		w
		return *this;
	}

	~Vec4<T>()
	{
	}

	// Vector Arithmetic Operators
	Vec4<T> operator+(const Vec4<T>& other) const
	{
		Vec4<T> temp(*this);
		temp += other;
		return temp;
	}
	Vec4<T> operator-(const Vec4<T>& other) const
	{
		Vec4<T> temp(*this);
		temp -= other;
		return temp;
	}
	Vec4<T>& operator+=(const Vec4<T>& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Vec4<T>& operator-=(const Vec4<T>& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	// No need to override * and / operators, we have a templated function for it
	Vec4<T>& operator*=(const Vec4<T>& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	Vec4<T>& operator/=(const Vec4<T>& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	// Scalar Arithmetic Operators
	template<class E> Vec4<T> operator*(E scalar) const
	{
		Vec4<T> temp(*this);
		temp *= scalar;
		return temp;
	}
	template<class E> Vec4<T> operator/(E scalar) const
	{
		Vec4<T> temp(*this);
		temp /= scalar;
		return temp;
	}
	template<class E> Vec4<T> operator*=(E scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	template<class E> Vec4<T> operator/=(E scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Inverse
	Vec4<T> operator-() const
	{
		Vec4 temp(*this);
		temp.x = -temp.x;
		temp.y = -temp.y;
		temp.z = -temp.z;
		return temp;
	}

	// Access/Write by index
	T operator[](int index) const
	{
		return (*this)[index];
	}
	T& operator[](int index)
	{
		if (index < 0 || index >= 4)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + index);
		}
		return data[index];
	}

	// Relational Operators
	bool operator==(const Vec4<T>& other)
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vec4<T>& other)
	{
		return x != other.x || y != other.y || z != other.z;
	}

	// Helper methods (to avoid instantiating new vectors)
	Vec4<T>& Add(T dx, T dy, T dz)
	{
		x += dx;
		y += dy;
		z += dz;

		return *this;
	}
	Vec4<T>& Subtract(T dx, T dy, T dz)
	{
		x -= dx;
		y -= dy;
		z -= dz;

		return *this;
	}

	// Other methods
	[[nodiscard]] bool IsEmpty() const
	{
		return !x && !y && !z;

	}
	[[nodiscard]] float Magnitude() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}
	Vec4<T>& Normalize()
	{
		*this /= Magnitude();
		return *this;
	}
	Vec4<T> GetNormalized() const
	{
		return Vec4(*this).Normalize();
	}
	Vec4<T>& Midpoint()
	{
		*this /= T(2);
		return *this;
	}
	Vec4<T> GetMidpoint() const
	{
		return Vec4(*this).Midpoint();
	}
	Vec4<T>& Clear()
	{
		x = 0;
		y = 0;
		z = 0;
		return *this;
	}
	Vec4<T>& SetMagnitude(T magnitude)
	{
		*this *= magnitude / Magnitude();
		return *this;
	}

	// Rotation functions
	Vec4<T>& Reflect(const Vec4<T>& normal)
	{
		// normal should be normalized already
		float factor = 2.0f * Dot(*this, normal);
		this->Subtract(normal.x * factor, normal.y * factor, normal.z * factor);
		return *this;
	}
	Vec4<T>& RotateX(const float angle)
	{
		float cos = std::cos(angle);
		float sin = std::sin(angle);

		T j = cos * y - sin * z;
		T k = sin * y + cos * z;

		y = j;
		z = k;
		return *this;
	}
	Vec4<T>& RotateY(const float angle)
	{
		float cos = std::cos(angle);
		float sin = std::sin(angle);

		T i = cos * x + sin * z;
		T k = -sin * x + cos * z;

		x = i;
		z = k;
		return *this;
	}
	Vec4<T>& RotateZ(const float angle)
	{
		float cos = std::cos(angle);
		float sin = std::sin(angle);

		T i = cos * x - sin * y;
		T j = sin * x + cos * y;

		x = i;
		y = j;
		return *this;
	}
	Vec4<T>& Rotate(const Vec4<T>& axis, const float angle)
	{
		// axis should be normalized already!
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		float dot = Dot(*this, axis);

		T i = axis.x * dot * (1.0f - cos) + x * cos + (-axis.z * y + axis.y * z) * sin;
		T j = axis.y * dot * (1.0f - cos) + y * cos + (axis.z * x - axis.x * z) * sin;
		T k = axis.z * dot * (1.0f - cos) + z * cos + (-axis.y * x + axis.x * y) * sin;

		x = i;
		y = j;
		z = k;
		return *this;
	}
};

template <typename T> [[nodiscard]]
T Dot(const Vec4<T>& left, const Vec4<T>& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T> [[nodiscard]]
float Angle(const Vec4<T>& left, const Vec4<T>& right)
{
	const float total = Dot(left, right) / (left.Magnitude() * right.Magnitude());
	return std::acos(total);
}

template <typename T> [[nodiscard]]
Vec4<T> Cross(const Vec4<T>& left, const Vec4<T>& right)
{
	T x = left.y * right.z - right.y * left.z;
	T y = left.z * right.x - right.z * left.x;
	T z = left.x * right.y - right.x * left.y;

	return Vec4<T>(x, y, z);
}

template <typename T> [[nodiscard]]
Vec4<T> Min(const Vec4<T>& left, const Vec4<T>& right)
{
	return Vec4<T>(
		(std::numeric_limits<T>::min)(left.x, right.x),
		(std::numeric_limits<T>::min)(left.y, right.y),
		(std::numeric_limits<T>::min)(left.z, right.z)
		);
}

template <typename T> [[nodiscard]]
Vec4<T> Max(const Vec4<T>& left, const Vec4<T>& right)
{
	return Vec4<T>(
		(std::numeric_limits<T>::max)(left.x, right.x),
		(std::numeric_limits<T>::max)(left.y, right.y),
		(std::numeric_limits<T>::max)(left.z, right.z)
		);
}

// Writing a vector to a string stream
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vec4<T>& vector)
{
	stream << "Vec4(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return stream;
}

// Default vector implementations
typedef Vec4<unsigned char> Vec4b;
typedef Vec4<int>           Vec4i;
typedef Vec4<float>         Vec4f;
typedef Vec4<double>        Vec4d;