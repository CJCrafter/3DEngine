#pragma once
#include <sstream>
#include "ArithmeticException.h"

template <int rows, int cols, class T>
struct Matrix
{

	T data[cols][rows];

	// Constructors
	Matrix()
	{
	}

	explicit Matrix(T fill)
	{
		Fill(fill);
	}

	Matrix(T data[cols][rows])
	{
		std::memcpy(this->data, data, cols);
	}

	// Matrix "Presets"
	Matrix<rows, cols, T>& Zero()
	{
		Fill(T(0));
		return *this;
	}

	Matrix<rows, cols, T>& Fill(T value)
	{

		for (int col = 0; col < cols; ++col)
		{
			for (int row = 0; row < rows; ++row)
			{
				data[row][col] = value;
			}
		}
		return *this;
	}

	Matrix<rows, cols, T>& Identity()
	{
		if (rows != cols) {
			std::ostringstream stream;
			stream << rows << " x " << cols << " Matrix";
			throw ArithmeticException(__LINE__, __FILE__, stream.str().c_str(), "rows != cols");
		}

		for (int i = 0; i < rows; i++)
		{
			data[i][i] = T(1);
		}

		return *this;
	}

	T* Raw()
	{
		return reinterpret_cast<T*>(data);
	}

	// Arithmatic Operators
	Matrix<rows, cols, T> operator+(const Matrix<rows, cols, T>& other) const
	{
		Matrix<rows, cols, T> matrix(*this);
		matrix += other;
		return matrix;
	}

	Matrix<rows, cols, T> operator-(const Matrix<rows, cols, T>& other) const
	{
		Matrix<rows, cols, T> matrix(*this);
		matrix -= other;
		return matrix;
	}

	Matrix<rows, cols, T> operator*(const Matrix<rows, cols, T>& other) const
	{
		Matrix<rows, cols, T> matrix(*this);
		matrix *= other;
		return matrix;
	}

	Matrix<rows, cols, T> operator/(const Matrix<rows, cols, T>& other) const
	{
		Matrix<rows, cols, T> matrix(*this);
		matrix /= other;
		return matrix;
	}

	Matrix<rows, cols, T>& operator+=(const Matrix<rows, cols, T>& other)
	{
	}

	Matrix<rows, cols, T>& operator-=(const Matrix<rows, cols, T>& other)
	{
	}

	Matrix<rows, cols, T>& operator*=(const Matrix<rows, cols, T>& other)
	{
	}

	Matrix<rows, cols, T>& operator/=(const Matrix<rows, cols, T>& other)
	{
	}

	// Scalars
	Matrix<rows, cols, T> operator*(T scalar) const
	{
		Matrix<rows, cols, T> matrix(*this);
		matrix *= scalar;
		return matrix;
	}

	Matrix<rows, cols, T> operator/(T scalar) const
	{
		Matrix<rows, cols, T> matrix(*this);
		matrix /= scalar;
		return matrix;
	}

	Matrix<rows, cols, T>& operator*=(T scalar)
	{
		for (int col = 0; col < cols; ++col)
		{
			for (int row = 0; row < rows; ++row)
			{
				data[col][row] *= scalar;
			}
		}

		return *this;
	}

	Matrix<rows, cols, T>& operator/=(T scalar)
	{
		for (int col = 0; col < cols; ++col)
		{
			for (int row = 0; row < rows; ++row)
			{
				data[col][row] /= scalar;
			}
		}

		return *this;
	}

	// Access Operators
	T get(int row, int col) const
	{
		if (row < 0 || row >= rows)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBoundsException", "Row is out of bounds: " + row);
		}
		if (col < 0 || col >= rows)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBoundsException", "Col is out of bounds: " + col);
		}

		return data[col][row];
	}

	void set(int row, int col, T value)
	{
		if (row < 0 || row >= rows)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBoundsException", "Row is out of bounds: " + row);
		}
		if (col < 0 || col >= rows)
		{
			throw EngineException(__LINE__, __FILE__, "IndexOutOfBoundsException", "Col is out of bounds: " + col);
		}

		data[col][row] = value;
	}

	// Equality Operators
	bool operator==(const Matrix<rows, cols, T>& other) const
	{
		// TODO std::tie?

		for (int col = 0; col < cols; col++)
		{
			for (int row = 0; row < rows; row++)
			{
				if (data[row][col] != other.data[row][col])
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator!=(const Matrix<rows, cols, T>& other) const
	{
		return !(*this == other);
	}
};

template <int rows, int cols, class T>
std::ostringstream& operator<<(std::ostringstream& stream, const Matrix<rows, cols, T>& right)
{
}

typedef Matrix<4, 4, float> Matrix4f;
