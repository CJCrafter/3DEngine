#include "Vector.h"
#include "EngineException.h"

template <int n, class T>
Vector<n, T>::Vector()
{
	Clear();
}

template <int n, class T>
Vector<n, T>::Vector(T value)
{
	for (int i = 0; i < n; i++)
		data[i] = value;
}

template <int n, class T>
Vector<n, T>::Vector(T data[n])
	:
	data(data)
{}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator+(const Vector<n, T>& other) const
{
	Vector<n, T> temp(this);
	temp += other;
	return temp;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator-(const Vector<n, T>& other) const
{
	Vector<n, T> temp(this);
	temp -= other;
	return temp;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator*(const Vector<n, T>& other) const
{
	Vector<n, T> temp(this);
	temp *= other;
	return temp;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator/(const Vector<n, T>& other) const
{
	Vector<n, T> temp(this);
	temp /= other;
	return temp;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::operator+=(const Vector<n, T>& other)
{
	for (int i = 0; i < n; i++)
		data[i] += other.data[i];
	
	return &this;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::operator-=(const Vector<n, T>& other)
{
	for (int i = 0; i < n; i++)
		data[i] -= other.data[i];

	return &this;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::operator*=(const Vector<n, T>& other)
{
	for (int i = 0; i < n; i++)
		data[i] *= other.data[i];

	return &this;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::operator/=(const Vector<n, T>& other)
{
	for (int i = 0; i < n; i++)
		data[i] /= other.data[i];

	return &this;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator*(T scalar) const
{
	Vector<n, T> temp(this);
	temp *= scalar;
	return temp;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator/(T scalar) const
{
	Vector<n, T> temp(this);
	temp /= scalar;
	return temp;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::operator*=(T scalar)
{
	for (int i = 0; i < n; i++)
		data[i] *= scalar;

	return &this;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::operator/=(T scalar)
{
	for (int i = 0; i < n; i++)
		data[i] /= scalar;

	return &this;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::operator-() const
{
	Vector<n, T> temp(this);
	for (int i = 0; i < n; i++)
		temp[i] = -temp[i];

	return temp;
}

template <int n, class T>
T Vector<n, T>::operator[](int index) const
{
	if (index < 0 || index >= n)
	{
		throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + index);
	}

	return data[index];
}

template <int n, class T>
bool Vector<n, T>::operator==(const Vector<n, T>& other) const
{
	return std::tie(data) == std::tie(other);
}

template <int n, class T>
bool Vector<n, T>::operator!=(const Vector<n, T>& other) const
{
	return !(this == other);
}

template <int n, class T>
bool Vector<n, T>::IsEmpty() const
{
	T zero = T(0);
	
	for (int i = 0; i < n; i++)
	{
		if (data[i] != zero)
			return false;
	}
	return true;
}

template <int n, class T>
T Vector<n, T>::Magnitude() const
{
	T total = T(0);
	for (int i = 0; i < n; i++)
		total += data[i] * data[i];
	
	return sqrt(total);
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::Normalize()
{
	T magnitude = Magnitude();
	for (int i = 0; i < n; i++)
		data[i] /= magnitude;
	
	return &this;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::GetNormalized() const
{
	return Vector(this).Normalize();
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::Midpoint()
{
	this /= T(2);
	return &this;
}

template <int n, class T>
Vector<n, T> Vector<n, T>::GetMidpoint() const
{
	Vector<n, T> temp(this);
	return temp.Midpoint();
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::Clear()
{
	for (int i = 0; i < n; i++)
		data[i] = 0;

	return &this;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::SetMagnitude(T magnitude)
{
	T scalar = magnitude / Magnitude();
	for (int i = 0; i < n; i++)
		data[i] *= scalar;
	
	return &this;
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::Rotate(const Vector<n, T>& axis, T theta)
{
}

template <int n, class T>
Vector<n, T>& Vector<n, T>::Reflect(const Vector<n, T>& normal)
{
	this -= 2 * Dot(this, normal) * normal;
	return &this;
}

template <int n, typename T>
T Dot(const Vector<n, T>& left, const Vector<n, T>& right)
{
	T total = T(0);
	for (int i = 0; i < n; i++)
		total += left[i] * right[i];

	return total;
}

template <int n, typename T>
double Angle(const Vector<n, T>& left, const Vector<n, T>& right)
{
	const double total = Dot(left, right) / (left.Magnitude() * right.Magnitude());
	return std::acos(total);
}

template <typename T>
Vector<2, T> Cross(const Vector<2, T>& vector)
{
	return Vector<2, T>(vector.y, -vector.x);
}

template <typename T>
Vector<3, T> Cross(const Vector<3, T>& left, const Vector<3, T>& right)
{
}

template <int n, typename T>
Vector<n, T> Cross(const Vector<n, T>* vectors[n - 1])
{
}

template <int n, typename T>
Vector<n, T> Min(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> temp;
	for (int i = 0; i < n; i++)
	{
		temp[i] = std::min(left[i], right[i]);
	}
	return temp;
}

template <int n, typename T>
Vector<n, T> Max(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> temp;
	for (int i = 0; i < n; i++)
	{
		temp[i] = std::max(left[i], right[i]);
	}
	return temp;
}

template <int n, typename T>
std::ostringstream& operator<<(std::ostringstream& stream, const Vector<n, T>& right)
{
	stream << "(";
	for (int i = 0; i < n; i++)
	{
		if (i != 0) stream << ",";

		stream << right[i];
	}
	stream << ")";
	return stream;
}