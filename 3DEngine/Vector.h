#pragma once
#include <sstream>

template<int n, class T>
struct Vector
{
	T data[n];

	Vector();
	explicit Vector(T value);
	Vector(T data[n]);

	// Arithmetic Operators
	Vector<n, T> operator+(const Vector<n, T>& other) const;
	Vector<n, T> operator-(const Vector<n, T>& other) const;
	Vector<n, T> operator*(const Vector<n, T>& other) const;
	Vector<n, T> operator/(const Vector<n, T>& other) const;
	Vector<n, T>& operator+=(const Vector<n, T>& other);
	Vector<n, T>& operator-=(const Vector<n, T>& other);
	Vector<n, T>& operator*=(const Vector<n, T>& other);
	Vector<n, T>& operator/=(const Vector<n, T>& other);

	// Never makes sense to add a scalar to a vector
	Vector<n, T> operator+(T) = delete;
	Vector<n, T> operator-(T) = delete;
	Vector<n, T>& operator+=(T) = delete;
	Vector<n, T>& operator-=(T) = delete;

	// Scalar Arithmetic Operators
	Vector<n, T> operator*(T scalar) const;
	Vector<n, T> operator/(T scalar) const;
	Vector<n, T>& operator*=(T scalar);
	Vector<n, T>& operator/=(T scalar);

	// Inverse Operator
	Vector<n, T> operator-() const;

	// Accessor by index. When using a 2d, 3d, or 4d vector, use the constants x, y, z, w
	T operator[](int index) const;
	T& operator[](int index);

	// Equality Operators
	bool operator==(const Vector<n, T>& other) const;
	bool operator!=(const Vector<n, T>& other) const;

	bool IsEmpty() const;
	
	// Magnitude Methods
	T Magnitude() const;
	Vector<n, T>& Normalize();
	Vector<n, T> GetNormalized() const;
	Vector<n, T>& Midpoint();
	Vector<n, T> GetMidpoint() const;
	Vector<n, T>& Clear();
	Vector<n, T>& SetMagnitude(T magnitude);
	
	// Rotation/Reflection Methods
	Vector<n, T>& Rotate(const Vector<n, T>& axis, T theta);
	Vector<n, T>& Reflect(const Vector<n, T>& normal); // The normal vector should be normalized
};

template <int n, typename T>
T Dot(const Vector<n, T>& left, const Vector<n, T>& right);

// We cannot use a template number here, we need floating points
template <int n, typename T>
double Angle(const Vector<n, T>& left, const Vector<n, T>& right);

// Invalid in the sense that you cannot take the cross-product of a 2 vector,
// but valid in the sense that this method will return a perpendicular vector
template <typename T>
Vector<2, T> Cross(const Vector<2, T>& vector);

template <typename T>
Vector<3, T> Cross(const Vector<3, T>& left, const Vector<3, T>& right);

template <int n, typename T>
Vector<n, T> Cross(const Vector<n, T>* vectors[n - 1]);

// Pairwise Min/Max
template <int n, typename T>
Vector<n, T> Min(const Vector<n, T>& left, const Vector<n, T>& right);

template <int n, typename T>
Vector<n, T> Max(const Vector<n, T>& left, const Vector<n, T>& right);

// Writing a vector to a string stream
template <int n, typename T>
std::ostringstream& operator<<(std::ostringstream& stream, const Vector<n, T>& right);

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