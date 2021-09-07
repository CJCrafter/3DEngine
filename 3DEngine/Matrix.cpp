#include "Matrix.h"
#include "ArithmeticException.h"

template<int rows, int cols, class T>
Matrix<rows, cols, T>::Matrix()
{
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>::Matrix(T fill)
{
	Fill(fill);
}

template <int rows, int cols, class T>
Matrix<rows, cols, T>::Matrix(T data[cols][rows])
	: data(data)
{}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::Zero()
{
	Fill(T(0));

	return &this;
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::Fill(T value)
{
	for (int row = 0; row < rows; row++) 
	{
		for (int col = 0; col < cols; col++) 
		{
			data[row][col] = value;
		}
	}

	return &this;
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::Identity()
{
	if (rows != cols) {
		std::ostringstream stream; 
		stream << rows << " x " << cols << " Matrix";
		throw ArithmeticException(__LINE__, __FILE__, stream.str().c_str(), "rows != cols");
	}

	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			data[row][col] = row == col ? T(1) : T(0);
		}
	}

	return &this;
}

template<int rows, int cols, class T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator+(const Matrix<rows, cols, T>& other) const
{
	return Matrix<rows, cols, T>();
}

template<int rows, int cols, class T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator-(const Matrix<rows, cols, T>& other) const
{
	return Matrix<rows, cols, T>();
}

template<int rows, int cols, class T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator*(const Matrix<rows, cols, T>& other) const
{
	return Matrix<rows, cols, T>();
}

template<int rows, int cols, class T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator/(const Matrix<rows, cols, T>& other) const
{
	return Matrix<rows, cols, T>();
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::operator+=(const Matrix<rows, cols, T>& other)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			data[row][col] += other[row][col];
		}
	}
	
	return &this;
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::operator-=(const Matrix<rows, cols, T>& other)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			data[row][col] -= other[row][col];
		}
	}

	return &this;
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::operator*=(const Matrix<rows, cols, T>& other)
{
	return &Matrix<rows, cols, T>();
}

template<int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::operator/=(const Matrix<rows, cols, T>& other)
{
	return &Matrix<rows, cols, T>();
}

template <int rows, int cols, class T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator*(T scalar) const
{
	return Matrix<rows, cols, T>();
}

template <int rows, int cols, class T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::operator/(T scalar) const
{
	return Matrix<rows, cols, T>();
}

template <int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::operator*=(T scalar)
{
	return &Matrix<rows, cols, T>();
}

template <int rows, int cols, class T>
Matrix<rows, cols, T>& Matrix<rows, cols, T>::operator/=(T scalar)
{
	return &Matrix<rows, cols, T>();
}

bool RangeCheck(const int index, const int bound)
{
	return index >= 0 && index < bound;
}

template <int rows, int cols, class T>
T Matrix<rows, cols, T>::get(int row, int col) const
{
	if (!RangeCheck(row, rows))
	{
		throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + row);
	}
	if (!RangeCheck(col, cols))
	{
		throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + cols);
	}

	return data[row][col];
}

template <int rows, int cols, class T>
void Matrix<rows, cols, T>::set(int row, int col, T value)
{
	if (!RangeCheck(row, rows))
	{
		throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + row);
	}
	if (!RangeCheck(col, cols))
	{
		throw EngineException(__LINE__, __FILE__, "IndexOutOfBounds", "For index: " + cols);
	}

	data[row][col] = value;
}

template <int rows, int cols, class T>
bool Matrix<rows, cols, T>::operator==(const Matrix<rows, cols, T>& other) const
{
	// TODO std::tie?
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (data[row][col] != other.data[row][col])
			{
				return false;
			}
		}
	}

	return true;
}

template <int rows, int cols, class T>
bool Matrix<rows, cols, T>::operator!=(const Matrix<rows, cols, T>& other) const
{
	return !(this == other);
}