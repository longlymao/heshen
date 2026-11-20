/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once
#include <cmath>
#include <span>

namespace rolling {

	template<typename T, size_t SIZE>
	class Vector
	{
		T data[SIZE];

	public:
        constexpr Vector() noexcept
            : data{}
        {}

        template<typename... Args>
        requires
            (sizeof...(Args) == SIZE) &&
            (std::is_convertible_v<Args, T> && ...)
        Vector(Args... args) noexcept
            : data{ static_cast<T>(args)... }
        {}

        template<size_t OtherSize>
        requires (OtherSize + 1 == SIZE)
        explicit Vector(const Vector<T, OtherSize>& other, T last) noexcept
        {
            for (size_t i = 0; i < OtherSize; ++i)
                data[i] = other[i];
            data[OtherSize] = last;
        }

		constexpr explicit Vector(std::span<const T, SIZE> span) noexcept
		{
			for (size_t i = 0; i < SIZE; ++i)
				data[i] = span[i];
		}

		~Vector() = default;
		Vector(const Vector& other) = default;
		Vector& operator=(const Vector& other) = default;

		constexpr static size_t Size() { return SIZE; }

		constexpr std::span<const T, SIZE> AsSpan() const
		{
			return std::span<const T, SIZE>(data, SIZE);
		}

		T& operator[](size_t index)
		{
			return data[index];
		}

		const T& operator[](size_t index) const
		{
			return data[index];
		}

		Vector operator+(const Vector& other) const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] + other[i];
			}
			return result;
		}

		Vector operator-(const Vector& other) const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] - other[i];
			}
			return result;
		}

		Vector operator*(T scalar) const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] * scalar;
			}
			return result;
		}

		Vector operator/(T scalar) const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] / scalar;
			}
			return result;
		}

		Vector operator+=(const Vector& other)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] += other[i];
			}
			return *this;
		}

		Vector operator-=(const Vector& other)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] -= other[i];
			}
			return *this;
		}

		Vector operator*=(T scalar)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] *= scalar;
			}
			return *this;
		}

		Vector operator/=(T scalar)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				data[i] /= scalar;
			}
			return *this;
		}

		Vector operator+() const
		{
			return *this;
		}

		Vector operator-() const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = -data[i];
			}
			return result;
		}

		T Dot(const Vector& other) const
		{
			T result = 0;
			for (size_t i = 0; i < SIZE; i++)
			{
				result += data[i] * other[i];
			}
			return result;
		}

		Vector Cross(const Vector& other) const
		{
			static_assert(SIZE == 3, "Cross product is only defined for 3D vectors");
			Vector result;
			result[0] = data[1] * other[2] - data[2] * other[1];
			result[1] = data[2] * other[0] - data[0] * other[2];
			result[2] = data[0] * other[1] - data[1] * other[0];
			return result;
		}

		Vector ComponentMultiply(const Vector& other) const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] * other[i];
			}
			return result;
		}

		Vector ComponentDivide(const Vector& other) const
		{
			Vector result;
			for (size_t i = 0; i < SIZE; i++)
			{
				result[i] = data[i] / other[i];
			}
			return result;
		}

		Vector& Normalize()
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

		void Swap(Vector& other)
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

}