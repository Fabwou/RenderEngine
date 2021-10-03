#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <iostream>

namespace engine {
	namespace core {
		namespace maths {

			struct Mat4;
			struct Vec3;

			struct Quaternion
			{
				//Components
				float m_w;
				float m_x;
				float m_y;
				float m_z;

				//Constructors/Destructors
				Quaternion();
				Quaternion(const Quaternion& lQuaternion);
				Quaternion(Quaternion&& rQuaternion) noexcept;
				Quaternion(float scalar);
				Quaternion(float w, float x, float y, float z);
				Quaternion(float scalar, const Vec3& vector);
				~Quaternion();

				//Shortcuts
				// Generates a Quaternion : ( 1 ; 0 ; 0 ; 0 ).
				static Quaternion Identity();

				//Tools
				static Quaternion Euler(const Vec3& eulerAngles);
				static Quaternion FromXAxis(const float angle);
				static Quaternion FromYAxis(const float angle);
				static Quaternion FromZAxis(const float angle);
				static Quaternion FromAxis(const Vec3& angles);
				static Quaternion Lerp(const Quaternion& leftQuat, const Quaternion& rightQuat, float ratio);
				static Quaternion NLerp(const Quaternion& leftQuat, const Quaternion& rightQuat, float ratio);
				static Quaternion Slerp(const Quaternion& leftQuat, const Quaternion& rightQuat, float ratio);

				//Operations
				bool Normalize();
				bool Inverse();
				Quaternion GetNormalized(bool* succed = nullptr) const;
				float GetModulus() const;
				float GetModulusSQ() const;
				float GetScalarPart() const;
				Vec3 GetVectorPart() const;
				Vec3 GetEulerAngles();
				Vec3 GetAxis() const;
				Quaternion GetConjugate() const;
				Quaternion GetInverse(bool* succed = nullptr) const;
				//Normalize quaternion before
				Mat4 GetRotationMatrix();
				//DOESN'T normalize quaternion before
				Mat4 GetRotationMatrix() const;
				void RotateVertex(Vec3& vertex) const;
				Vec3 GetRotatedVertex(const Vec3& vertex) const;
				float DotProduct(const Quaternion& quat) const;

				//Operators
				Quaternion& operator=	(const Quaternion& quaternion);
				Quaternion& operator=	(Quaternion && quaternion) noexcept;
				Quaternion& operator=	(float scalar);
				Quaternion	operator+	(float scalar)	const;
				Quaternion	operator+	(const Quaternion& quaternion)	const;
				Quaternion& operator+=	(float scalar);
				Quaternion& operator+=	(const Quaternion& quaternion);
				Quaternion& operator-	();
				Quaternion	operator-	(float				scalar)	const;
				Quaternion	operator-	(const Quaternion& quaternion)	const;
				Quaternion& operator-=	(float scalar);
				Quaternion& operator-=	(const Quaternion& quaternion);
				Quaternion	operator*	(float scalar)	const;
				Quaternion	operator*	(const Quaternion& quaternion)	const;
				Quaternion& operator*=	(float scalar);
				Quaternion& operator*=	(const Quaternion& quaternion);
				Quaternion	operator/	(float scalar)	const;
				Quaternion	operator/	(const Quaternion& quaternion)	const;
				Quaternion& operator/=	(float scalar);
				Quaternion& operator/=	(const Quaternion& quaternion);
				bool		operator==	(const Quaternion& quaternion)	const;
				bool		operator!=	(const Quaternion& quaternion)	const;
			};
			//External Operators
			std::ostream& operator<<(std::ostream&, const Quaternion&);
			Quaternion operator+(float scalar, const Quaternion&);
			Quaternion operator-(float scalar, const Quaternion&);
			Quaternion operator*(float scalar, const Quaternion&);
			Quaternion operator/(float scalar, const Quaternion&);

		}
	}
}

#endif