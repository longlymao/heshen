/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <cstring>
#include <cmath>
#include <utility>

namespace rolling {

	template<typename T, size_t SIZE>
	class Vector
	{
		T data[SIZE];
	public:
		Vector()
		{
			memset(data, 0, SIZE * sizeof(T));
		}
		template<typename... Args>
		Vector(Args... args)
		{
			static_assert(sizeof...(args) == SIZE, "Invalid number of arguments");
			T arr[] = { static_cast<T>(args)... };
			memcpy(data, arr, SIZE * sizeof(T));
		}
		T& operator[](size_t index)
		{
			return data[index];
		}
		const T& operator[](size_t index) const
		{
			return data[index];
		}
		Vector<T, SIZE> operator+(const Vector<T, SIZE>& other) const
		{
			Vector<T, SIZE> result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] + other[i];
			}
			return result;
		}
		Vector<T, SIZE> operator*(T scalar) const
		{
			Vector<T, SIZE> result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] * scalar;
			}
			return result;
		}
		Vector<T, SIZE> operator-(const Vector<T, SIZE>& other) const
		{
			Vector<T, SIZE> result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] - other[i];
			}
			return result;
		}
		Vector<T, SIZE> operator/(T scalar) const
		{
			Vector<T, SIZE> result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] / scalar;
			}
			return result;
		}
		Vector<T, SIZE> operator-() const
		{
			Vector<T, SIZE> result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = -data[i];
			}
			return result;
		}
		Vector<T, SIZE> operator+=(const Vector<T, SIZE>& other)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] += other[i];
			}
			return *this;
		}
		Vector<T, SIZE> operator-=(const Vector<T, SIZE>& other)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] -= other[i];
			}
			return *this;
		}
		Vector<T, SIZE> operator*=(T scalar)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] *= scalar;
			}
			return *this;
		}
		Vector<T, SIZE> operator/=(T scalar)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] /= scalar;
			}
			return *this;
		}
		T Dot(const Vector<T, SIZE>& other) const
		{
			T result = 0;
			for (size_t i = 0; i < SIZE; i++)
			{
				result += data[i] * other[i];
			}
			return result;
		}
		Vector<T, SIZE> Cross(const Vector<T, SIZE>& other) const
		{
			static_assert(SIZE == 3, "Cross product is only defined for 3D vectors");
			Vector<T, SIZE> result;
			result[0] = data[1] * other[2] - data[2] * other[1];
			result[1] = data[2] * other[0] - data[0] * other[2];
			result[2] = data[0] * other[1] - data[1] * other[0];
			return result;
		}
		Vector<T, SIZE>& Normalize()
		{
			T length = 0;
			for (size_t i = 0; i < SIZE; i++)
			{
				length += data[i] * data[i];
			}
			length = sqrt(length);
			if (length > 0)
			{
				for (size_t i = 0; i < SIZE; i++)
				{
					data[i] /= length;
				}
			}
			return *this;
		}
		void Swap(Vector<T, SIZE>& other)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				std::swap(data[i], other[i]);
			}
		}

		bool IsZero() const
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				if (data[i] != 0)
					return false;
			}
			return true;
		}

		T Length() const
		{
			T length = 0;
			for (size_t i = 0; i < SIZE; i++)
			{
				length += data[i] * data[i];
			}
			return sqrt(length);
		}
	};

	typedef Vector<float, 2> VectorF2;
	typedef Vector<float, 3> VectorF3;
	typedef Vector<float, 4> VectorF4;

	template<typename T, size_t ROW, size_t COL>
	class Matrix
	{
		using thisType = Matrix<T, ROW, COL>;

		T data[ROW][COL];
	public:
		Matrix()
		{
			memset(data, 0, ROW * COL * sizeof(T));
		}

		~Matrix() = default;

		Matrix(const thisType& other) = default;
		thisType& operator=(const thisType& other) = default;

		T* operator[](size_t index)
		{
			return data[index];
		}

		const T* operator[](size_t index) const
		{
			return data[index];
		}

		void SetRow(size_t row, const Vector<T, COL>& vector)
		{
			for (size_t i = 0; i < COL; i++)
			{
				data[row][i] = vector[i];
			}
		}
		void SetRow(size_t row, const Vector<T, COL - 1>& vector, T tail)
		{
			for (size_t i = 0; i < COL - 1; i++)
			{
				data[row][i] = vector[i];
			}
			data[row][COL - 1] = tail;
		}
		void SetRow(size_t row, const Vector<T, COL - 1>& vector)
		{
			for (size_t i = 0; i < COL - 1; i++)
			{
				data[row][i] = vector[i];
			}
		}
		void SetColumn(size_t col, const Vector<T, ROW>& vector)
		{
			for (size_t i = 0; i < ROW; i++)
			{
				data[i][col] = vector[i];
			}
		}
		void SetColumn(size_t col, const Vector<T, ROW - 1>& vector, T tail)
		{
			for (size_t i = 0; i < ROW - 1; i++)
			{
				data[i][col] = vector[i];
			}
			data[ROW - 1][col] = tail;
		}
		void SetColumn(size_t col, const Vector<T, ROW - 1>& vector)
		{
			for (size_t i = 0; i < ROW - 1; i++)
			{
				data[i][col] = vector[i];
			}
		}

		Matrix<T, ROW, COL> operator+(const Matrix<T, ROW, COL>& other) const
		{
			Matrix<T, ROW, COL> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] + other[i][j];
				}
			}
			return result;
		}
		Matrix<T, ROW, COL> operator*(T scalar) const
		{
			Matrix<T, ROW, COL> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] * scalar;
				}
			}
			return result;
		}
		Matrix<T, ROW, COL> operator-(const Matrix<T, ROW, COL>& other) const
		{
			Matrix<T, ROW, COL> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] - other[i][j];
				}
			}
			return result;
		}
		Matrix<T, ROW, COL> operator/(T scalar) const
		{
			Matrix<T, ROW, COL> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] / scalar;
				}
			}
			return result;
		}
		Matrix<T, ROW, COL> operator-() const
		{
			Matrix<T, ROW, COL> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = -data[i][j];
				}
			}
			return result;
		}
		Matrix<T, ROW, COL> operator+=(const Matrix<T, ROW, COL>& other)
		{
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					data[i][j] += other[i][j];
				}
			}
			return *this;
		}

		template<size_t COL2>
		thisType operator*(const Matrix<T, COL, COL2>& other) const
		{
			Matrix<T, ROW, COL2> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL2; j++)
				{
					result[i][j] = 0;
					for (size_t k = 0; k < COL; k++)
					{
						result[i][j] += data[i][k] * other[k][j];
					}
				}
			}
			return result;
		}

		Vector<T, ROW> operator*(const Vector<T, COL>& vector) const
		{
			Vector<T, ROW> result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i] += data[i][j] * vector[j];
				}
			}
			return result;
		}

		constexpr static auto Identity() {
			thisType mat;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					if (i == j)
					{
						mat[i][j] = 1;
					}
				}
			}
			return mat;
		}
	};

	using Matrix4x4 = Matrix<float, 4, 4>;

}