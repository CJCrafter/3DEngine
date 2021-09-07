#pragma once
#include <sstream>

template <int rows, int cols, class T>
struct Matrix
{
	T data[cols][rows];

	// Constructors
	Matrix();
	explicit Matrix(T fill);
	Matrix(T data[cols][rows]);

	// Matrix "Presets"
	Matrix<rows, cols, T>& Zero();
	Matrix<rows, cols, T>& Fill(T value);
	Matrix<rows, cols, T>& Identity();

	// Arithmatic Operators
	Matrix<rows, cols, T> operator+(const Matrix<rows, cols, T>& other) const;
	Matrix<rows, cols, T> operator-(const Matrix<rows, cols, T>& other) const;
	Matrix<rows, cols, T> operator*(const Matrix<rows, cols, T>& other) const;
	Matrix<rows, cols, T> operator/(const Matrix<rows, cols, T>& other) const;
	Matrix<rows, cols, T>& operator+=(const Matrix<rows, cols, T>& other);
	Matrix<rows, cols, T>& operator-=(const Matrix<rows, cols, T>& other);
	Matrix<rows, cols, T>& operator*=(const Matrix<rows, cols, T>& other);
	Matrix<rows, cols, T>& operator/=(const Matrix<rows, cols, T>& other);

	// Scalars
	Matrix<rows, cols, T> operator*(T scalar) const;
	Matrix<rows, cols, T> operator/(T scalar) const;
	Matrix<rows, cols, T>& operator*=(T scalar);
	Matrix<rows, cols, T>& operator/=(T scalar);

	// Access Operators
	T get(int row, int col) const;
	void set(int row, int col, T value);

	// Equality Operators
	bool operator==(const Matrix<rows, cols, T>& other) const;
	bool operator!=(const Matrix<rows, cols, T>& other) const;
};

template <int rows, int cols, class T>
std::ostringstream& operator<<(std::ostringstream& stream, const Matrix<rows, cols, T>& right);

typedef Matrix<4, 4, double> Matrix4d;