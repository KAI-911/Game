#pragma once
#include <cstdint>
#include <math.h>
#include "Math.h"
#include "Vector3.h"
#include "MathDefine.h"

namespace application
{
	namespace math
	{

		class Matrix4x4
		{
		public:
			float matrix[4][4];

		public:
			constexpr Matrix4x4() noexcept
				: matrix{
					1.0f,0.0f,0.0f,0.0f,
					0.0f,1.0f,0.0f,0.0f,
					0.0f,0.0f,1.0f,0.0f,
					0.0f,0.0f,0.0f,1.0f}
			{}

			Matrix4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept
				: matrix{m00, m01, m02, m03,m10, m11, m12, m13,m20, m21, m22, m23,m30, m31, m32, m33}
			{}

			/*
			 * @brief		転置行列を返す
			 */
			inline Matrix4x4 GetTransposeMatrix() const noexcept
			{
				return Matrix4x4(
					matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
					matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
					matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
					matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]
				);
			}

			/// <summary>
			/// 逆行列を返す
			/// </summary>
			/// <returns></returns>
			inline Matrix4x4 GetInverseMatrix()const
			{
				Matrix4x4 inv;//ここに逆行列が入る
				Matrix4x4 mat = *this;//ここに逆行列が入る
				float tmp = 0; //一時的なデータを蓄える
				uint32_t i, j, k; //カウンタ
				const uint32_t n = 4;  //配列の次数

				//掃き出し法
				for(i = 0; i < n; i++){
					tmp = 1 / mat.matrix[i][i];
					for(j = 0; j < n; j++){
						mat.matrix[i][j] *= tmp;
						inv.matrix[i][j] *= tmp;
					}
					for(j = 0; j < n; j++){
						if(i != j){
							tmp = mat.matrix[j][i];
							for(k = 0; k < n; k++){
								mat.matrix[j][k] -= mat.matrix[i][k] * tmp;
								inv.matrix[j][k] -= inv.matrix[i][k] * tmp;
							}
						}
					}
				}
				return inv;
			}

			/// <summary>
			/// 移動行列を作成
			/// </summary>
			/// <param name="_x">x方向の移動量</param>
			/// <param name="_y">y方向の移動量</param>
			/// <param name="_z">z方向の移動量</param>
			inline void SetTransformMatrix(const float _x, const float _y, const float _z)
			{
				matrix[0][0] = 1;
				matrix[0][1] = 0;
				matrix[0][2] = 0;
				matrix[0][3] = 0;

				matrix[1][0] = 0;
				matrix[1][1] = 1;
				matrix[1][2] = 0;
				matrix[1][3] = 0;

				matrix[2][0] = 0;
				matrix[2][1] = 0;
				matrix[2][2] = 1;
				matrix[2][3] = 0;

				matrix[3][0] = _x;
				matrix[3][1] = _y;
				matrix[3][2] = _z;
				matrix[3][3] = 1;
			}

			/// <summary>
			/// 移動行列を返す
			/// </summary>
			/// <param name="_x">x方向の移動量</param>
			/// <param name="_y">y方向の移動量</param>
			/// <param name="_z">z方向の移動量</param>
			/// <returns></returns>
			inline static Matrix4x4 CreateTransformMatrix(const float _x, const float _y, const float _z)
			{
				Matrix4x4 ret;
				ret.SetTransformMatrix(_x, _y, _z);
				return ret;
			}

			/// <summary>
			/// X回転行列を作成
			/// </summary>
			/// <param name="angle">回転角度</param>
			void SetRotateXMatrix(const float angle)
			{
				matrix[0][0] = 1;
				matrix[0][1] = 0;
				matrix[0][2] = 0;
				matrix[0][3] = 0;

				matrix[1][0] = 0;
				matrix[1][1] = cosf(AngleToRadian(angle));
				matrix[1][2] = sinf(AngleToRadian(angle));
				matrix[1][3] = 0;

				matrix[2][0] = 0;
				matrix[2][1] = -sinf(AngleToRadian(angle));
				matrix[2][2] = cosf(AngleToRadian(angle));
				matrix[2][3] = 0;

				matrix[3][0] = 0;
				matrix[3][1] = 0;
				matrix[3][2] = 0;
				matrix[3][3] = 1;
			}

			/// <summary>
			/// Y回転行列を作成
			/// </summary>
			/// <param name="angle">回転角度</param>
			void SetRotateYMatrix(const float angle)
			{
				matrix[0][0] = cosf(AngleToRadian(angle));
				matrix[0][1] = 0;
				matrix[0][2] = -sinf(AngleToRadian(angle));
				matrix[0][3] = 0;

				matrix[1][0] = 0;
				matrix[1][1] = 1;
				matrix[1][2] = 0;
				matrix[1][3] = 0;

				matrix[2][0] = sinf(AngleToRadian(angle));
				matrix[2][1] = 0;
				matrix[2][2] = cosf(AngleToRadian(angle));
				matrix[2][3] = 0;

				matrix[3][0] = 0;
				matrix[3][1] = 0;
				matrix[3][2] = 0;
				matrix[3][3] = 1;
			}

			/// <summary>
			/// Z回転行列を作成
			/// </summary>
			/// <param name="angle">回転角度</param>
			void SetRotateZMatrix(const float angle)
			{
				matrix[0][0] = cosf(AngleToRadian(angle));
				matrix[0][1] = sinf(AngleToRadian(angle));
				matrix[0][2] = 0;
				matrix[0][3] = 0;

				matrix[1][0] = -sinf(AngleToRadian(angle));
				matrix[1][1] = cosf(AngleToRadian(angle));
				matrix[1][2] = 0;
				matrix[1][3] = 0;

				matrix[2][0] = 0;
				matrix[2][1] = 0;
				matrix[2][2] = 1;
				matrix[2][3] = 0;

				matrix[3][0] = 0;
				matrix[3][1] = 0;
				matrix[3][2] = 0;
				matrix[3][3] = 1;
			}

			 /// <summary>
			 /// 指定軸回転行列を作成
			 /// </summary>
			 /// <param name="axis">回転軸</param>
			 /// <param name="angle">回転角度</param>
			void SetRotateAxisMatrix(const Vector3& axis, const float angle)
			{
				float cos = cosf(AngleToRadian(angle));
				float sin = sinf(AngleToRadian(angle));
				float invcos = 1 - cos;
				matrix[0][0] = axis.x * axis.x * invcos + cos;
				matrix[0][1] = axis.x * axis.y * invcos - axis.z * sin;
				matrix[0][2] = axis.x * axis.z * invcos + axis.y * sin;

				matrix[1][0] = axis.x * axis.y * invcos + axis.z * sin;
				matrix[1][1] = axis.y * axis.y * invcos + cos;
				matrix[1][2] = axis.y * axis.z * invcos - axis.x * sin;

				matrix[2][0] = axis.x * axis.z * invcos - axis.y * sin;
				matrix[2][1] = axis.y * axis.z * invcos + axis.x * sin;
				matrix[2][2] = axis.z * axis.z * invcos + cos;
			}

			/// <summary>
			/// X回転行列を返す
			/// </summary>
			/// <param name="angle">回転角度</param>
			/// <returns></returns>
			static Matrix4x4 CreateRotationXMatrix(const float angle)
			{
				Matrix4x4 ret;
				ret.SetRotateXMatrix(angle);
				return ret;
			}

			/// <summary>
			/// Y回転行列を返す
			/// </summary>
			/// <param name="angle">回転角度</param>
			/// <returns></returns>
			static Matrix4x4 CreateRotationYMatrix(const float angle)
			{
				Matrix4x4 ret;
				ret.SetRotateYMatrix(angle);
				return ret;
			}

			/// <summary>
			/// Z回転行列を返す
			/// </summary>
			/// <param name="angle">回転角度</param>
			/// <returns></returns>
			static Matrix4x4 CreateRotationZMatrix(const float angle)
			{
				Matrix4x4 ret;
				ret.SetRotateZMatrix(angle);
				return ret;
			}

			/// <summary>
			/// 指定軸回転行列を返す
			/// </summary>
			/// <param name="axis">回転軸</param>
			/// <param name="angle">回転角度</param>
			/// <returns></returns>
			static Matrix4x4 CreateRotationAxisMatrix(const Vector3& axis, const float angle)
			{
				Matrix4x4 ret;
				ret.SetRotateAxisMatrix(axis, angle);
				return ret;
			}

			/// <summary>
			/// ベクトルと行列を掛ける
			/// </summary>
			/// <param name="v">変更前のベクトル</param>
			/// <returns>変更後のベクトル</returns>
			Vector3 Mull(const Vector3& v)
			{
				Vector3 ret;
				ret.x = matrix[0][0] * v.x + matrix[1][0] * v.y + matrix[2][0] * v.z + matrix[3][0];
				ret.y = matrix[0][1] * v.x + matrix[1][1] * v.y + matrix[2][1] * v.z + matrix[3][1];
				ret.z = matrix[0][2] * v.x + matrix[1][2] * v.y + matrix[2][2] * v.z + matrix[3][2];
				return ret;
			}

			/// <summary>
			/// ベクトルと行列を掛ける
			/// </summary>
			/// <param name="v">変更前のベクトル</param>
			/// <param name="m">計算する行列</param>
			/// <returns>変更後のベクトル</returns>
			static Vector3 Mull(const Vector3& v, const Matrix4x4& m)
			{
				Vector3 ret;
				ret.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
				ret.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
				ret.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];
				return ret;
			}

			Matrix4x4& operator+=(const Matrix4x4& m) noexcept
			{
				matrix[0][0] += m.matrix[0][0];
				matrix[0][1] += m.matrix[0][1];
				matrix[0][2] += m.matrix[0][2];
				matrix[0][3] += m.matrix[0][3];
				matrix[1][0] += m.matrix[1][0];
				matrix[1][1] += m.matrix[1][1];
				matrix[1][2] += m.matrix[1][2];
				matrix[1][3] += m.matrix[1][3];
				matrix[2][0] += m.matrix[2][0];
				matrix[2][1] += m.matrix[2][1];
				matrix[2][2] += m.matrix[2][2];
				matrix[2][3] += m.matrix[2][3];
				matrix[3][0] += m.matrix[3][0];
				matrix[3][1] += m.matrix[3][1];
				matrix[3][2] += m.matrix[3][2];
				matrix[3][3] += m.matrix[3][3];

				return *this;
			}
			Matrix4x4 operator+(const Matrix4x4& m) const noexcept
			{
				return Matrix4x4(
					matrix[0][0] + m.matrix[0][0],
					matrix[0][1] + m.matrix[0][1],
					matrix[0][2] + m.matrix[0][2],
					matrix[0][3] + m.matrix[0][3],
					matrix[1][0] + m.matrix[1][0],
					matrix[1][1] + m.matrix[1][1],
					matrix[1][2] + m.matrix[1][2],
					matrix[1][3] + m.matrix[1][3],
					matrix[2][0] + m.matrix[2][0],
					matrix[2][1] + m.matrix[2][1],
					matrix[2][2] + m.matrix[2][2],
					matrix[2][3] + m.matrix[2][3],
					matrix[3][0] + m.matrix[3][0],
					matrix[3][1] + m.matrix[3][1],
					matrix[3][2] + m.matrix[3][2],
					matrix[3][3] + m.matrix[3][3]
				);
			}
			Matrix4x4& operator-=(const Matrix4x4& m) noexcept
			{
				matrix[0][0] -= m.matrix[0][0];
				matrix[0][1] -= m.matrix[0][1];
				matrix[0][2] -= m.matrix[0][2];
				matrix[0][3] -= m.matrix[0][3];
				matrix[1][0] -= m.matrix[1][0];
				matrix[1][1] -= m.matrix[1][1];
				matrix[1][2] -= m.matrix[1][2];
				matrix[1][3] -= m.matrix[1][3];
				matrix[2][0] -= m.matrix[2][0];
				matrix[2][1] -= m.matrix[2][1];
				matrix[2][2] -= m.matrix[2][2];
				matrix[2][3] -= m.matrix[2][3];
				matrix[3][0] -= m.matrix[3][0];
				matrix[3][1] -= m.matrix[3][1];
				matrix[3][2] -= m.matrix[3][2];
				matrix[3][3] -= m.matrix[3][3];

				return *this;
			}
			Matrix4x4 operator-(const Matrix4x4& m) const noexcept
			{
				return Matrix4x4(
					matrix[0][0] - m.matrix[0][0],
					matrix[0][1] - m.matrix[0][1],
					matrix[0][2] - m.matrix[0][2],
					matrix[0][3] - m.matrix[0][3],
					matrix[1][0] - m.matrix[1][0],
					matrix[1][1] - m.matrix[1][1],
					matrix[1][2] - m.matrix[1][2],
					matrix[1][3] - m.matrix[1][3],
					matrix[2][0] - m.matrix[2][0],
					matrix[2][1] - m.matrix[2][1],
					matrix[2][2] - m.matrix[2][2],
					matrix[2][3] - m.matrix[2][3],
					matrix[3][0] - m.matrix[3][0],
					matrix[3][1] - m.matrix[3][1],
					matrix[3][2] - m.matrix[3][2],
					matrix[3][3] - m.matrix[3][3]
				);
			}
			Matrix4x4& operator*=(const Matrix4x4& m) noexcept
			{
				*this = *this * m;

				return *this;
			}
			Matrix4x4 operator*(const Matrix4x4& m) const noexcept
			{
				return Matrix4x4(
					matrix[0][0] * m.matrix[0][0] +
					matrix[0][1] * m.matrix[1][0] +
					matrix[0][2] * m.matrix[2][0] +
					matrix[0][3] * m.matrix[3][0],
					matrix[0][0] * m.matrix[0][1] +
					matrix[0][1] * m.matrix[1][1] +
					matrix[0][2] * m.matrix[2][1] +
					matrix[0][3] * m.matrix[3][1],
					matrix[0][0] * m.matrix[0][2] +
					matrix[0][1] * m.matrix[1][2] +
					matrix[0][2] * m.matrix[2][2] +
					matrix[0][3] * m.matrix[3][2],
					matrix[0][0] * m.matrix[0][3] +
					matrix[0][1] * m.matrix[1][3] +
					matrix[0][2] * m.matrix[2][3] +
					matrix[0][3] * m.matrix[3][3],

					matrix[1][0] * m.matrix[0][0] +
					matrix[1][1] * m.matrix[1][0] +
					matrix[1][2] * m.matrix[2][0] +
					matrix[1][3] * m.matrix[3][0],
					matrix[1][0] * m.matrix[0][1] +
					matrix[1][1] * m.matrix[1][1] +
					matrix[1][2] * m.matrix[2][1] +
					matrix[1][3] * m.matrix[3][1],
					matrix[1][0] * m.matrix[0][2] +
					matrix[1][1] * m.matrix[1][2] +
					matrix[1][2] * m.matrix[2][2] +
					matrix[1][3] * m.matrix[3][2],
					matrix[1][0] * m.matrix[0][3] +
					matrix[1][1] * m.matrix[1][3] +
					matrix[1][2] * m.matrix[2][3] +
					matrix[1][3] * m.matrix[3][3],

					matrix[2][0] * m.matrix[0][0] +
					matrix[2][1] * m.matrix[1][0] +
					matrix[2][2] * m.matrix[2][0] +
					matrix[2][3] * m.matrix[3][0],
					matrix[2][0] * m.matrix[0][1] +
					matrix[2][1] * m.matrix[1][1] +
					matrix[2][2] * m.matrix[2][1] +
					matrix[2][3] * m.matrix[3][1],
					matrix[2][0] * m.matrix[0][2] +
					matrix[2][1] * m.matrix[1][2] +
					matrix[2][2] * m.matrix[2][2] +
					matrix[2][3] * m.matrix[3][2],
					matrix[2][0] * m.matrix[0][3] +
					matrix[2][1] * m.matrix[1][3] +
					matrix[2][2] * m.matrix[2][3] +
					matrix[2][3] * m.matrix[3][3],

					matrix[3][0] * m.matrix[0][0] +
					matrix[3][1] * m.matrix[1][0] +
					matrix[3][2] * m.matrix[2][0] +
					matrix[3][3] * m.matrix[3][0],
					matrix[3][0] * m.matrix[0][1] +
					matrix[3][1] * m.matrix[1][1] +
					matrix[3][2] * m.matrix[2][1] +
					matrix[3][3] * m.matrix[3][1],
					matrix[3][0] * m.matrix[0][2] +
					matrix[3][1] * m.matrix[1][2] +
					matrix[3][2] * m.matrix[2][2] +
					matrix[3][3] * m.matrix[3][2],
					matrix[3][0] * m.matrix[0][3] +
					matrix[3][1] * m.matrix[1][3] +
					matrix[3][2] * m.matrix[2][3] +
					matrix[3][3] * m.matrix[3][3]
				);
			}
			const float* operator[](uint32_t row)const
			{
				return matrix[row];
			}
			float* operator[](uint32_t row)
			{
				return matrix[row];
			}
		};

	}
}