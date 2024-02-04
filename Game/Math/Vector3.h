#pragma once
#include <math.h>
namespace application
{
	namespace math
	{

		class Vector3
		{
		public:
			float x;
			float y;
			float z;

		public:
			Vector3() :x(0.0f), y(0.0f), z(0.0f) {};
			Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};
			Vector3(float value[3]) :x(value[0]), y(value[1]), z(value[2]) {};

			/// <summary>
			/// 正規化ベクトルを返す
			/// </summary>
			/// <returns></returns>
			inline Vector3 Nomarize()const
			{
				Vector3 ret = *this / Length();
				return ret;
			}

			/// <summary>
			/// 内積を求める
			/// </summary>
			/// <param name="vector">比較するベクトル</param>
			/// <returns>内積</returns>
			inline float Dot(const Vector3& vector)const
			{
				return x * vector.x + y * vector.y + z * vector.z;
			}

			/// <summary>
			/// 内積を求める
			/// </summary>
			/// <param name="vector1">比較するベクトル</param>
			/// <param name="vector2">比較するベクトル</param>
			/// <returns>内積</returns>
			inline static float Dot(const Vector3& vector1, const Vector3& vector2)
			{
				return vector1.Dot(vector2);
			}

			/// <summary>
			/// 外積を求める
			/// </summary>
			/// <param name="vector">比較するベクトル</param>
			/// <returns>外積</returns>
			inline Vector3 Cross(const Vector3& vector)const
			{
				return Vector3(
					y * vector.z - z * vector.y,
					z * vector.x - x * vector.z,
					x * vector.y - y * vector.x
				);
			}

			/// <summary>
			/// 外積を求める
			/// </summary>
			/// <param name="vector1">比較するベクトル</param>
			/// <param name="vector2">比較するベクトル</param>
			/// <returns>外積</returns>
			inline static Vector3 Cross(const Vector3& vector1, const Vector3& vector2)
			{
				return vector1.Cross(vector2);
			}

			/// <summary>
			/// ベクトルの長さを返す
			/// </summary>
			/// <returns></returns>
			inline float Length()const
			{
				return sqrtf(SquareLength());
			}

			/// <summary>
			/// ベクトルの長さの2乗を返す
			/// </summary>
			/// <returns></returns>
			inline float SquareLength()const
			{
				return x * x + y * y + z * z;
			}

			/// <summary>
			/// 反射ベクトルを返す
			/// </summary>
			/// <param name="n">反射する壁の法線</param>
			/// <returns></returns>
			inline Vector3 Reflection(const Vector3& n) const
			{
				return *this + n * (-2.0f * Dot(n));
			}

			/// <summary>
			/// 壁に沿ったベクトルを返す
			/// </summary>
			/// <param name="n">壁の法線</param>
			/// <returns></returns>
			inline Vector3 Slid(const Vector3& n) const
			{
				return *this + n * (-1.0f * Dot(n));
			}

			inline Vector3 operator+ (const Vector3& right)const
			{
				Vector3 ret;
				ret.x = x + right.x;
				ret.y = y + right.y;
				ret.z = z + right.z;
				return ret;
			}

			inline Vector3& operator+= (const Vector3& right)
			{
				x = x + right.x;
				y = y + right.y;
				z = z + right.z;
				return *this;
			}

			inline Vector3 operator- (const Vector3& right)const
			{
				Vector3 ret;
				ret.x = x - right.x;
				ret.y = y - right.y;
				ret.z = z - right.z;
				return ret;
			}

			inline Vector3& operator-= (const Vector3& right)
			{
				x = x - right.x;
				y = y - right.y;
				z = z - right.z;
				return *this;
			}

			inline Vector3 operator* (const float scale)const
			{
				Vector3 ret;
				ret.x = x * scale;
				ret.y = y * scale;
				ret.z = z * scale;
				return ret;
			}

			inline Vector3& operator*= (const float scale)
			{
				x = x * scale;
				y = y * scale;
				z = z * scale;
				return *this;
			}

			inline Vector3 operator/ (const float scale)const
			{
				Vector3 ret;
				ret.x = x / scale;
				ret.y = y / scale;
				ret.z = z / scale;
				return ret;
			}

			inline Vector3& operator/= (const float scale)
			{
				x = x / scale;
				y = y / scale;
				z = z / scale;
				return *this;
			}
		};

	}
}