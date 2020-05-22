#pragma once
#include "Types.h"
#include <cmath>

namespace Bs::Math
{
	template <typename T, i32 Dimensions>
	struct Vector
	{
		T data[Dimensions];

		constexpr Vector() = default;

		constexpr explicit Vector(T value)
		{
			for (i32 i = 0; i < Dimensions; ++i)
			{
				data[i] = value;
			}
		}

		constexpr Vector(std::initializer_list<T> ilist)
		{
			constexpr int i = 0;
			for (const auto& elem : ilist)
			{
				data[i] = elem;
				++i;
			}
		}
	};

	template <typename T>
	struct Vector<T, 2> {
		union {
			T data[2];
			struct { T x, y; };
		};

		constexpr Vector() = default;

		constexpr explicit Vector(T value)
		{
			x = value;
			y = value;
		}

		constexpr Vector(std::initializer_list<T> ilist)
		{
			this->x = ilist.begin()[0];
			this->y = ilist.begin()[1];
		}

		T SquareMagnitude() const
		{
			return ((x * x) + (y * y));
		}

		f32 Magnitude() const
		{
			return static_cast<f32>(std::sqrt(SquareMagnitude()));
		}

		Vector<f32, 2> Normalize() const
		{
			f32 magnitude = Magnitude();
			return Vector<f32, 2>
			{
				x / magnitude,
					y / magnitude
			};
		}
	};

	template <typename T>
	struct Vector<T, 3> {
		union {
			T data[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			Vector<T, 2> xy;
		};

		constexpr Vector() = default;

		constexpr Vector(T value)
		{
			x = value;
			y = value;
			z = value;
		}

		constexpr Vector(std::initializer_list<T> ilist)
		{
			this->x = ilist.begin()[0];
			this->y = ilist.begin()[1];
			this->z = ilist.begin()[2];
		}

		T SquareMagnitude() const
		{
			return ((x * x) + (y * y) + (z * z));
		}

		f32 Magnitude() const
		{
			return static_cast<f32>(std::sqrt(SquareMagnitude()));
		}

		Vector<f32, 3> Normalize() const
		{
			f32 magnitude = Magnitude();
			return Vector<f32, 3>
			{
				x / magnitude,
					y / magnitude,
					z / magnitude
			};
		}
	};

	template <typename T>
	struct Vector<T, 4> {
		union {
			T data[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			Vector<T, 2> xy;
			Vector<T, 3> xyz;
			Vector<T, 3> rgb;
		};

		constexpr Vector() = default;

		constexpr Vector(T value)
		{
			x = value;
			y = value;
			z = value;
			w = value;
		}

		constexpr Vector(std::initializer_list<T> ilist)
		{
			this->x = ilist.begin()[0];
			this->y = ilist.begin()[1];
			this->z = ilist.begin()[2];
			this->w = ilist.begin()[3];
		}

		T SquareMagnitude() const
		{
			return ((x * x) + (y * y) + (z * z) + (w * w));
		}

		f32 Magnitude() const
		{
			return static_cast<f32>(std::sqrt(SquareMagnitude()));
		}

		Vector<f32, 4> Normalize() const
		{
			f32 magnitude = Magnitude();
			return Vector<f32, 4>
			{
				x / magnitude,
					y / magnitude,
					z / magnitude,
					w / magnitude
			};
		}
	};

	template<typename T, i32 Dimensions>
	Vector<T, Dimensions> operator+(const Vector<T, Dimensions>& a, Vector<T, Dimensions>& b)
	{
		Vector<T, Dimensions> result;
		for (i32 i = 0; i < Dimensions; ++i)
		{
			result.data[i] = a.data[i] + b.data[i];
		}
		return result;
	}

	template<typename T, i32 Dimensions>
	Vector<T, Dimensions> operator-(const Vector<T, Dimensions>& a, Vector<T, Dimensions>& b)
	{
		Vector<T, Dimensions> result;
		for (i32 i = 0; i < Dimensions; ++i)
		{
			result.data[i] = a.data[i] - b.data[i];
		}
		return result;
	}

	template<typename T, i32 Dimensions>
	Vector<T, Dimensions> operator*(const Vector<T, Dimensions>& a, Vector<T, Dimensions>& b)
	{
		Vector<T, Dimensions> result;
		for (i32 i = 0; i < Dimensions; ++i)
		{
			result.data[i] = a.data[i] * b.data[i];
		}
		return result;
	}

	template<typename T, i32 Dimensions>
	Vector<T, Dimensions> operator*(const Vector<T, Dimensions>& vec, const T& scalar)
	{
		Vector<T, Dimensions> result;
		for (i32 i = 0; i < Dimensions; ++i)
		{
			result.data[i] = vec.data[i] * scalar;
		}
		return result;
	}

	template<typename T, i32 Dimensions>
	Vector<T, Dimensions> operator*(const T& scalar, const Vector<T, Dimensions>& vec)
	{
		return vec * scalar;
	}

	/*
	* Returns `0` when two vectors are perpendicular, `> 0` when two vectors are in
	* the same general direction, `1` when two *normalized* vectors are parallel,
	* `< 0` when two vectors are in opposite general direction and `-1` when two
	* *normalized* vectors are antiparallel.
	*/
	template<typename T, i32 Dimensions>
	T Dot(const Vector<T, Dimensions>& a, const Vector<T, Dimensions>& b)
	{
		T result{};
		for (i32 i = 0; i < Dimensions; ++i)
		{
			result += a.data[i] * b.data[i];
		}
		return result;
	}

	template<typename T>
	Vector<T, 3> Cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
	{
		return Vector<T, 3>
		{
			a.y* b.z - a.z * b.y,
				a.z* b.x - a.x * b.z,
				a.x* b.y - a.y * b.x,
		};
	}

	template<typename T>
	using Vec1 = Vector<T, 1>;

	template<typename T>
	using Vec2 = Vector<T, 2>;

	template<typename T>
	using Vec3 = Vector<T, 3>;

	template<typename T>
	using Vec4 = Vector<T, 4>;
}
