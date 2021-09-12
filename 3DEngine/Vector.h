#pragma once
#include <sstream>

template<int n, class T>
struct Vector
{
	T data[n];

	void Fill(T value)
	{
		for (int i = 0; i < n; i++)
			data[i] = value;
	}

	// Arithmetic Operators
	Vector<n, T> operator+(const Vector<n, T>& other) const
	{
		Vector<n, T> temp(this);
		temp += other;
		return temp;
	}
	Vector<n, T> operator-(const Vector<n, T>& other) const
	{
		Vector<n, T> temp(this);
		temp -= other;
		return temp;
	}
	Vector<n, T> operator*(const Vector<n, T>& other) const
	{
		Vector<n, T> temp(this);
		temp *= other;
		return temp;
	}
	Vector<n, T> operator/(const Vector<n, T>& other) const
	{
		Vector<n, T> temp(this);
		temp /= other;
		return temp;
	}
	Vector<n, T>& operator+=(const Vector<n, T>& other)
	{
		for (int i = 0; i < n; i++)
			data[i] += other.data[i];

		return &this;
	}
	Vector<n, T>& operator-=(const Vector<n, T>& other)
	{
		for (int i = 0; i < n; i++)
			data[i] -= other.data[i];

		return &this;
	}
	Vector<n, T>& operator*=(const Vector<n, T>& other)
	{
		for (int i = 0; i < n; i++)
			data[i] *= other.data[i];

		return &this;
	}
	Vector<n, T>& operator/=(const Vector<n, T>& other)
	{
		for (int i = 0; i < n; i++)
			data[i] /= other.data[i];

		return &this;
	}

	// Never makes sense to add a scalar to a vector
	Vector<n, T> operator+(T) = delete;
	Vector<n, T> operator-(T) = delete;
	Vector<n, T>& operator+=(T) = delete;
	Vector<n, T>& operator-=(T) = delete;

	// Scalar Arithmetic Operators
	Vector<n, T> operator*(T scalar) const
	{
		Vector<n, T> temp(this);
		temp *= scalar;
		return temp;
	}
	Vector<n, T> operator/(T scalar) const
	{
		Vector<n, T> temp(this);
		temp /= scalar;
		return temp;
	}
	Vector<n, T>& operator*=(T scalar)
	{
		for (int i = 0; i < n; i++)
			data[i] *= scalar;

		return &this;
	}
	Vector<n, T>& operator/=(T scalar)
	{
		for (int i = 0; i < n; i++)
			data[i] /= scalar;

		return &this;
	}

	// Inverse Operator
	Vector<n, T> operator-() const
	{
		Vector<n, T> temp(this);
		for (int i = 0; i < n; i++)
			temp[i] = -temp[i];

		return temp;
	}

	// Accessor by index. When using a 2d, 3d, or 4d vector, use the constants x, y, z, w
	T operator[](int index) const
	{
		if (index < 0 || index >= n)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + index);
		}

		return data[index];
	}
	T& operator[](int index)
	{
		if (index < 0 || index >= n)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + index);
		}

		return data[index];
	}

	// Equality Operators
	bool operator==(const Vector<n, T>& other) const
	{
		return std::tie(data) == std::tie(other);
	}
	bool operator!=(const Vector<n, T>& other) const
	{
		return !(this == other);
	}

	[[nodiscard]] bool IsEmpty() const
	{
		T zero = T(0);

		for (int i = 0; i < n; i++)
		{
			if (data[i] != zero)
				return false;
		}
		return true;
	}
	
	// Magnitude Methods
	[[nodiscard]] T Magnitude() const
	{
		T total = T(0);
		for (int i = 0; i < n; i++)
			total += data[i] * data[i];

		return sqrt(total);
	}
	Vector<n, T>& Normalize()
	{
		T magnitude = Magnitude();
		for (int i = 0; i < n; i++)
			data[i] /= magnitude;

		return &this;
	}
	[[nodiscard]] Vector<n, T> GetNormalized() const
	{
		return Vector(this).Normalize();
	}
	Vector<n, T>& Midpoint()
	{
		this /= T(2);
		return &this;
	}
	[[nodiscard]] Vector<n, T> GetMidpoint() const
	{
		Vector<n, T> temp(this);
		return temp.Midpoint();
	}
	Vector<n, T>& Clear()
	{
		for (int i = 0; i < n; i++)
			data[i] = 0;

		return &this;
	}
	Vector<n, T>& SetMagnitude(T magnitude)
	{
		T scalar = magnitude / Magnitude();
		for (int i = 0; i < n; i++)
			data[i] *= scalar;

		return &this;
	}
	
	// Rotation/Reflection Methods
	Vector<n, T>& Rotate(const Vector<n, T>& axis, T theta);
	Vector<n, T>& Reflect(const Vector<n, T>& normal); // The normal vector should be normalized
};

template <int n, typename T> 
T Dot(Vector<n, T> left, Vector<n, T> right)
{
	T total = T(0);
	for (int i = 0; i < n; i++)
		total += left[i] * right[i];

	return total;
}

// We cannot use a template number here, we need floating points
template <int n, typename T> [[nodiscard]]
double Angle(const Vector<n, T>& left, const Vector<n, T>& right)
{
	const double total = Dot(left, right) / (left.Magnitude() * right.Magnitude());
	return std::acos(total);
}

// Invalid in the sense that you cannot take the cross-product of a 2 vector,
// but valid in the sense that this method will return a perpendicular vector
template <typename T> [[nodiscard]]
Vector<2, T> Cross(const Vector<2, T>& vector)
{
	return Vector<2, T>(vector.y, -vector.x);
}

template <typename T> [[nodiscard]]
Vector<3, T> Cross(const Vector<3, T>& left, const Vector<3, T>& right)
{
	return Vector<3, T>();
}

// Pairwise Min/Max
template <int n, typename T> [[nodiscard]]
Vector<n, T> Min(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> temp;
	for (int i = 0; i < n; i++)
	{
		temp[i] = std::min(left[i], right[i]);
	}
	return temp;
}

template <int n, typename T> [[nodiscard]]
Vector<n, T> Max(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> temp;
	for (int i = 0; i < n; i++)
	{
		temp[i] = std::max(left[i], right[i]);
	}
	return temp;
}

// Writing a vector to a string stream
template <int n, typename T>
std::ostringstream& operator<<(std::ostringstream& stream, const Vector<n, T>& right)
{
	stream << "(";
	for (int i = 0; i < n; i++)
	{
		if (i != 0) stream << ", ";

		T element = right[i];
		stream << right[i];
	}
	stream << ")";
	return stream;
}

// Default vector definitions
typedef Vector<2, int>    Vec2i;
typedef Vector<3, int>    Vec3i;
typedef Vector<2, float>  Vec2f;
typedef Vector<3, float>  Vec3f;
typedef Vector<2, double> Vec2d;
typedef Vector<3, double> Vec3d;

// Allows accessing of values by x, y, z instead of [0], [1], [2]
template <class T> struct Vector<2, T> { union { float data[2]; struct { T x, y; }; }; };
template <class T> struct Vector<3, T> { union { float data[3]; struct { T x, y, z; }; }; };
template <class T> struct Vector<4, T> { union { float data[4]; struct { T x, y, z, w; }; }; };