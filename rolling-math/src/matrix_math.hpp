/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once
#include "vector_math.hpp"

namespace rolling {

	template<typename T, size_t ROW, size_t COL>
	class Matrix
	{
		T data[ROW][COL];

	public:
        using RowView = std::span<T, COL>;
	    using ConstRowView = std::span<const T, COL>;

		constexpr static size_t Rows() { return ROW; }
		constexpr static size_t Cols() { return COL; }

		constexpr Matrix() noexcept
			: data{}
		{}

		Matrix(const Matrix& other) = default;
		Matrix& operator=(const Matrix& other) = default;

		constexpr RowView operator[](size_t index)
		{
			return RowView(data[index], COL);
		}

		constexpr ConstRowView operator[](size_t index) const
		{
			return ConstRowView{data[index], COL};
		}

		constexpr Vector<T, COL> GetRow(size_t row) const
		{
			return Vector<T, COL>((*this)[row]);
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

		Matrix operator+(const Matrix& other) const
		{
			Matrix result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] + other[i][j];
				}
			}
			return result;
		}

		Matrix operator*(T scalar) const
		{
			Matrix result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] * scalar;
				}
			}
			return result;
		}

		Matrix operator-(const Matrix& other) const
		{
			Matrix result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] - other[i][j];
				}
			}
			return result;
		}
		
		Matrix operator/(T scalar) const
		{
			Matrix result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = data[i][j] / scalar;
				}
			}
			return result;
		}

		Matrix operator-() const
		{
			Matrix result;
			for (size_t i = 0; i < ROW; i++)
			{
				for (size_t j = 0; j < COL; j++)
				{
					result[i][j] = -data[i][j];
				}
			}
			return result;
		}

		Matrix operator+=(const Matrix& other)
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
		Matrix operator*(const Matrix<T, COL, COL2>& other) const
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
			Matrix mat;
			for (size_t i = 0; i < std::min(ROW, COL); i++)
			{
				mat[i][i] = 1;
			}
			return mat;
		}
	};

	using Matrix4x4 = Matrix<float, 4, 4>;

}