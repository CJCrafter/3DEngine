#pragma once
#include <sstream>

#include "EngineException.h"

template<class T>
struct Vec3
{
	union
	{
		T data[3]; struct { T x, y, z; };
	};

	Vec3()
		: x(0),
		  y(0),
		  z(0)
	{
	}
	Vec3(T x, T y, T z)
		: x(x),
		  y(y),
		  z(z)
	{
	}

	Vec3(const Vec3& other)
		: x(other.x),
		  y(other.y),
		  z(other.z)
	{
	}

	Vec3(Vec3&& other) noexcept
		: x(std::move(other.x)),
		  y(std::move(other.y)),
		  z(std::move(other.z))
	{
	}

	Vec3<T>& operator=(const Vec3<T>& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vec3<T>& operator=(Vec3<T>&& other) noexcept
	{
		if (this == &other)
			return *this;
		x = std::move(other.x);
		y = std::move(other.y);
		z = std::move(other.z);
		return *this;
	}

	~Vec3<T>()
	{
	}

	// Vector Arithmetic Operators
	Vec3<T> operator+(const Vec3<T>& other) const
	{
		Vec3<T> temp(*this);
		temp += other;
		return temp;
	}
	Vec3<T> operator-(const Vec3<T>& other) const
	{
		Vec3<T> temp(*this);
		temp -= other;
		return temp;
	}
	Vec3<T>& operator+=(const Vec3<T>& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	Vec3<T>& operator-=(const Vec3<T>& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	// No need to override * and / operators, we have a templated function for it
	Vec3<T>& operator*=(const Vec3<T>& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	Vec3<T>& operator/=(const Vec3<T>& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	// Scalar Arithmetic Operators
	template<class E> Vec3<T> operator*(E scalar) const
	{
		Vec3<T> temp(*this);
		temp *= scalar;
		return temp;
	}
	template<class E> Vec3<T> operator/(E scalar) const
	{
		Vec3<T> temp(*this);
		temp /= scalar;
		return temp;
	}
	template<class E> Vec3<T> operator*=(E scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	template<class E> Vec3<T> operator/=(E scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	// Inverse
	Vec3<T> operator-() const
	{
		return Vec3<T>(-x, -y, -z);
	}

	// Access/Write by index
	T operator[](int index) const
	{
		return (*this)[index];
	}
	T& operator[](int index)
	{
		if (index < 0 || index >= 3)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + index);
		}
		return data[index];
	}

	// Relational Operators
	bool operator==(const Vec3<T>& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vec3<T>& other)
	{
		return x != other.x || y != other.y || z != other.z;
	}

	// Helper methods (to avoid instantiating new vectors)
	Vec3<T>& Add(T dx, T dy, T dz)
	{
		x += dx;
		y += dy;
		z += dz;

		return *this;
	}
	Vec3<T>& Subtract(T dx, T dy, T dz)
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
	[[nodiscard]] float MagnitudeSquared() const
	{
		return x * x + y * y + z * z;
	}
	Vec3<T>& Normalize()
	{
		*this /= Magnitude();
		return *this;
	}
	Vec3<T> GetNormalized() const
	{
		return Vec3(*this).Normalize();
	}
	Vec3<T>& Midpoint()
	{
		*this /= T(2);
		return *this;
	}
	Vec3<T> GetMidpoint() const
	{
		return Vec3(*this).Midpoint();
	}
	Vec3<T>& Clear()
	{
		x = 0;
		y = 0;
		z = 0;
		return *this;
	}
	Vec3<T>& SetMagnitude(T magnitude)
	{
		*this *= magnitude / Magnitude();
		return *this;
	}

	// Rotation functions
	Vec3<T>& Reflect(const Vec3<T>& normal)
	{
		// normal should be normalized already
		float factor = 2.0f * Dot(*this, normal);
		this->Subtract(normal.x * factor, normal.y * factor, normal.z * factor);
		return *this;
	}
	Vec3<T>& RotateX(const float angle)
	{
		float cos = std::cos(angle);
		float sin = std::sin(angle);

		T j = cos * y - sin * z;
		T k = sin * y + cos * z;

		y = j;
		z = k;
		return *this;
	}
	Vec3<T>& RotateY(const float angle)
	{
		float cos = std::cos(angle);
		float sin = std::sin(angle);

		T i = cos * x + sin * z;
		T k = -sin * x + cos * z;

		x = i;
		z = k;
		return *this;
	}
	Vec3<T>& RotateZ(const float angle)
	{
		float cos = std::cos(angle);
		float sin = std::sin(angle);

		T i = cos * x - sin * y;
		T j = sin * x + cos * y;

		x = i;
		y = j;
		return *this;
	}
	Vec3<T>& Rotate(const Vec3<T>& axis, const float angle)
	{
		// axis should be normalized already!
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		float dot = Dot(*this, axis);

		T i = axis.x * dot * (1.0f - cos) + x * cos + (-axis.z * y + axis.y * z) * sin;
		T j = axis.y * dot * (1.0f - cos) + y * cos + ( axis.z * x - axis.x * z) * sin;
		T k = axis.z * dot * (1.0f - cos) + z * cos + (-axis.y * x + axis.x * y) * sin;

		x = i;
		y = j;
		z = k;
		return *this;
	}
};

template <typename T> [[nodiscard]]
T Dot(const Vec3<T>& left, const Vec3<T>& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T> [[nodiscard]]
float Angle(const Vec3<T>& left, const Vec3<T>& right)
{
	const float total = Dot(left, right) / (left.Magnitude() * right.Magnitude());
	return std::acos(total);
}

template <typename T> [[nodiscard]]
Vec3<T> Cross(const Vec3<T>& left, const Vec3<T>& right)
{
	T x = left.y * right.z - right.y * left.z;
	T y = left.z * right.x - right.z * left.x;
	T z = left.x * right.y - right.x * left.y;

	return Vec3<T>(x, y, z);
}

template <typename T> [[nodiscard]]
Vec3<T> Min(const Vec3<T>& left, const Vec3<T>& right)
{
	return Vec3<T>(
		(std::numeric_limits<T>::min)(left.x, right.x), 
		(std::numeric_limits<T>::min)(left.y, right.y),
		(std::numeric_limits<T>::min)(left.z, right.z)
		);
}

template <typename T> [[nodiscard]]
Vec3<T> Max(const Vec3<T>& left, const Vec3<T>& right)
{
	return Vec3<T>(
		(std::numeric_limits<T>::max)(left.x, right.x),
		(std::numeric_limits<T>::max)(left.y, right.y),
		(std::numeric_limits<T>::max)(left.z, right.z)
		);
}

// Writing a vector to a string stream
template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vec3<T>& vector)
{
	stream << "Vec3(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return stream;
}

// Default vector implementations
typedef Vec3<unsigned char> Vec3b;
typedef Vec3<int>           Vec3i;
typedef Vec3<float>         Vec3f;
typedef Vec3<double>        Vec3d;