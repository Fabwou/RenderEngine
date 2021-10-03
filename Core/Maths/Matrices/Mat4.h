#ifndef _MAT4_H_
#define _MAT4_H_

#include "../Vectors/Vec3/Vec3.h"
#include "../Vectors/Vec4/Vec4.h"
#include "../Tools/Axis.h"

namespace engine
{
	namespace core
	{
		namespace maths
		{
			struct Quaternion;

			struct Mat4
			{
				float m_matrix[4][4];

				//Constructors/Destructors
				Mat4();
				Mat4(const Mat4& other);
				Mat4(Mat4 && other) noexcept;
				Mat4(float aa, float ab, float ac, float ad,
					float ba, float bb, float bc, float bd,
					float ca, float cb, float cc, float cd,
					float da, float db, float dc, float dd);
				Mat4(float matrix[4][4]);
				~Mat4();

				//Shortcuts
				// Generates a matrix full of 0
				static Mat4 Zero();
				// Generates an identity matrix
				static Mat4 Identity();

				//Tools
				static Mat4 TRS(const Vec3& translation, const Quaternion& rotation, const Vec3& scale);
				static Mat4 CreateTranslationMatrix(const Vec3& translation);
				static Mat4 CreateRotationMatrix(EAxis axis, float angle);
				static Mat4 CreateRotationMatrix(const Quaternion& rotation);
				static Mat4 CreateScaleMatrix(const Vec3& scale);

				//Operations
				// Sets all values to 0
				void Clear();
				// Changes the matrix into a unitary matrix
				void Init();
				void Transpose();
				Mat4 GetTranspose() const;
				bool IsOrthogonal() const;
				float GetDeterminant() const;
				bool Reverse();
				Mat4 GetInverse(bool* succed = nullptr) const;
				// Uses 'out' as a buffer (faster)
				bool GetInverseByRef(Mat4& out) const;

				//Operators
				Mat4& operator=(const Mat4& mat);
				Mat4& operator=(Mat4 && mat) noexcept;
				Mat4 operator+(const Mat4 & mat) const;
				Mat4& operator+=(const Mat4 & mat);
				Mat4 operator-() const;
				Mat4 operator-(const Mat4 & mat) const;
				Mat4& operator-=(const Mat4 & mat);
				Mat4 operator*(float scalar) const;
				Mat4 operator*(const Mat4 & mat) const;
				Vec4 operator*(const Vec4 & vec) const;
				Mat4& operator*=(float scalar);
				Mat4& operator*=(const Mat4 & mat);
				bool operator==(const Mat4 & mat) const;
				bool operator!=(const Mat4 & mat) const;
				// /!\ Should be used with caution!
				// Returns the associated component.
				float* operator[](int index);
				// (read only)
				const float* operator[](int index) const;
			};
			//External Operator
			std::ostream& operator<<(std::ostream& stream, const Mat4& mat);
			Mat4 operator*(float scalar, const Mat4& mat);
		}
	}
};
#endif
