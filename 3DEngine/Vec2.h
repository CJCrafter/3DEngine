#pragma once
#include <sstream>

#include "EngineException.h"

template<class T>
struct Vec2
{
	union
	{
		T data[2]; struct { T x, y; };
	};

	Vec2()
		: x(0),
		  y(0)
	{
	}
	Vec2(T x, T y)
		: x(x),
		  y(y)
	{
	}

	Vec2(const Vec2& other)
		: x(other.x),
		  y(other.y)
	{
	}

	Vec2(Vec2&& other) noexcept
		: x(std::move(other.x)),
		  y(std::move(other.y))
	{
	}

	Vec2<T>& operator=(const Vec2<T>& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		return *this;
	}

	Vec2<T>& operator=(Vec2<T>&& other) noexcept
	{
		if (this == &other)
			return *this;
		x = std::move(other.x);
		y = std::move(other.y);
		return *this;
	}

	~Vec2<T>()
	{
		delete[] data;
	}

	// Vector Arithmetic Operators
	Vec2<T> operator+(const Vec2<T>& other) const
	{
		Vec2<T> temp(*this);
		temp += other;
		return temp;
	}
	Vec2<T> operator-(const Vec2<T>& other) const
	{
		Vec2<T> temp(*this);
		temp -= other;
		return temp;
	}
	Vec2<T>& operator+=(const Vec2<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	Vec2<T>& operator-=(const Vec2<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// Scalar Arithmetic Operators
	template<class E> Vec2<T> operator*(E scalar) const
	{
		Vec2<T> temp(this);
		temp *= scalar;
		return temp;
	}
	template<class E> Vec2<T> operator/(E scalar) const
	{
		Vec2<T> temp(this);
		temp /= scalar;
		return temp;
	}
	template<class E> Vec2<T> operator*=(E scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}
	template<class E> Vec2<T> operator/=(E scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	// Inverse
	Vec2<T> operator-() const
	{
		Vec2 temp(*this);
		temp.x = -temp.x;
		temp.y = -temp.y;
		return temp;
	}

	// Access/Write by index
	T operator[](int index) const
	{
		return (*this)[index];
	}
	T& operator[](int index)
	{
		if (index < 0 || index >= 2)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + index);
		}
		return data[index];
	}

	// Relational Operators
	bool operator==(const Vec2<T>& other)
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vec2<T>& other)
	{
		return x != other.x || y != other.y;
	}

	// Helper methods (to avoid instantiating new vectors)
	Vec2<T>& Add(T dx, T dy)
	{
		x += dx;
		y += dy;

		return *this;
	}
	Vec2<T>& Subtract(T dx, T dy)
	{
		x -= dx;
		y -= dy;

		return *this;
	}

	// Other methods
	[[nodiscard]] bool IsEmpty() const
	{
		return !x && !y;

	}
	[[nodiscard]] float Magnitude() const
	{
		return std::sqrt(x * x + y * y);
	}
	Vec2<T>& Normalize()
	{
		*this /= Magnitude();
		return *this;
	}
	Vec2<T> GetNormalized() const
	{
		return Vec2(*this).Normalize();
	}
	Vec2<T>& Midpoint()
	{
		*this /= T(2);
		return *this;
	}
	Vec2<T> GetMidpoint() const
	{
		return Vec2(*this).Midpoint();
	}
	Vec2<T>& Clear()
	{
		x = T(0);
		y = T(0);
		return *this;
	}
	Vec2<T>& SetMagnitude(T magnitude)
	{
		*this *= magnitude / Magnitude();
		return *this;
	}
};

template <typename T> [[nodiscard]]
T Dot(const Vec2<T>& left, const Vec2<T>& right)
{
	return left.x * right.x + left.y * right.y;
}

template <typename T> [[nodiscard]]
float Angle(const Vec2<T>& left, const Vec2<T>& right)
{
	const float total = Dot(left, right) / (left.Magnitude() * right.Magnitude());
	return std::acos(total);
}

template <typename T> [[nodiscard]]
Vec2<T> Cross(const Vec2<T>& vector)
{
	return Vec2(vector.y, -vector.x);
}

template <typename T> [[nodiscard]]
Vec2<T> Min(const Vec2<T>& left, const Vec2<T>& right)
{
	return Vec2<T>(std::min(left.x, right.x), std::min(left.y, right.y));
}

template <typename T> [[nodiscard]]
Vec2<T> Max(const Vec2<T>& left, const Vec2<T>& right)
{
	return Vec2<T>(std::max(left.x, right.x), std::max(left.y, right.y));
}

// Writing a vector to a string stream
template <typename T>
std::ostringstream& operator<<(std::ostringstream& stream, const Vec2<T>& vector)
{
	stream << "Vec2(" << vector.x << ", " << vector.y << ")";
	return stream;
}

// Default vector implementations
typedef Vec2<unsigned char> Vec2b;
typedef Vec2<int>           Vec2i;
typedef Vec2<float>         Vec2f;
typedef Vec2<double>        Vec2d;