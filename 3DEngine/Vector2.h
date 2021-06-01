#pragma once

template<class T>
struct Vector2d
{
	T x;
	T y;

	Vector2d();
	Vector2d(const T& x, const T& y);

	// Arithmetic operators
	Vector2d<T>& operator+(const Vector2d<T>&);
	Vector2d<T>& operator-(const Vector2d<T>&);
	Vector2d<T>& operator*(const Vector2d<T>&);
	Vector2d<T>& operator/(const Vector2d<T>&);

	// Equality operator
	Vector2d<T>& operator==(const Vector2d<T>&);
	Vector2d<T>& operator!=(const Vector2d<T>&);
	
};
