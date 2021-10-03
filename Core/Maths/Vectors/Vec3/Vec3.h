#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>

namespace engine {
	namespace core {
		namespace maths {

			struct Vec3 {

				//Components
				float m_x;
				float m_y;
				float m_z;

				//Contructors/Destructors
				Vec3();
				Vec3(const Vec3& lVec3);
				Vec3(Vec3 && rVec3) noexcept;
				Vec3(float x, float y, float z);
				~Vec3();

				//Shortcuts
				// Generates a Vec3 : ( 0 ; 0 ; 0 ).
				static Vec3 Zero();
				// Generates a Vec3 : ( -1 ; 0 ; 0 ).
				static Vec3 Left();
				// Generates a Vec3 : ( 1 ; 0 ; 0 ).
				static Vec3 Right();
				// Generates a Vec3 : ( 0 ; -1 ; 0 ).
				static Vec3 Down();
				// Generates a Vec3 : ( 0 ; 1 ; 0 ).
				static Vec3 Up();
				// Generates a Vec3 : ( 0 ; 0 ; -1 ).
				static Vec3 Back();
				// Generates a Vec3 : ( 0 ; 0 ; 1 ).
				static Vec3 Forward();
				// Generates a Vec3 : ( 1 ; 1 ; 1 ).
				static Vec3 One();

				static Vec3 Lerp(const Vec3& firstVec, const Vec3& secondVec, float ratio);
				static Vec3 NLerp(const Vec3& firstVec, const Vec3& secondVec, float ratio);
				static bool CompareWithEpsilon(Vec3 a, Vec3 b, float epsilon = 0.00001f);

				//Operations
				void Normalize();
				Vec3 GetNormalized() const;
				float GetMagnitude() const;
				float GetMagnitudeSQ() const;
				float DotProduct(const Vec3&) const;
				Vec3 CrossProduct(const Vec3&) const;

				//Operators
				Vec3& operator=(const Vec3&);
				Vec3& operator=(Vec3&&) noexcept;
				Vec3 operator+(const Vec3&) const;
				Vec3& operator+=(const Vec3&);
				Vec3 operator-(const Vec3&) const;
				Vec3& operator-=(const Vec3&);
				Vec3 operator*(float) const;
				Vec3& operator*=(float);
				Vec3 operator/(float) const;
				Vec3& operator/=(float);
				bool operator==(const Vec3&) const;
				bool operator!=(const Vec3&) const;

			};

			//External operators
			std::ostream& operator<<(std::ostream&, const Vec3&);
			Vec3 operator*(float, const Vec3&);

		}
	}
}

#endif