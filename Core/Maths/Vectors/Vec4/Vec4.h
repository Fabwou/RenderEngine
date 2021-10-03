#ifndef _VEC4_H_
#define _VEC4_H_

#include <iostream>

namespace engine {
	namespace core {
		namespace maths {

			struct Vec4 {

				//Components
				float m_x;
				float m_y;
				float m_z;
				float m_w;

				//Constructors/Destructors
				Vec4();
				Vec4(const Vec4& lVec4);
				Vec4(Vec4&& rVec4) noexcept;
				Vec4(float x, float y, float z, float w = 1.f);
				~Vec4();

				//Shortcuts
				// Generates a Vec4 : ( 0 ; 0 ; 0 ; 1 ).
				static Vec4 Zero();
				// Generates a Vec4 : ( -1 ; 0 ; 0 ; 1 ).
				static Vec4 Left();
				// Generates a Vec4 : ( 1 ; 0 ; 0 ; 1 ).
				static Vec4 Right();
				// Generates a Vec4 : ( 0 ; -1 ; 0 ; 1 ).
				static Vec4 Down();
				// Generates a Vec4 : ( 0 ; 1 ; 0 ; 1 ).
				static Vec4 Up();
				// Generates a Vec4 : ( 0 ; 0 ; -1 ; 1 ).
				static Vec4 Back();
				// Generates a Vec4 : ( 0 ; 0 ; 1 ; 1 ).
				static Vec4 Forward();
				// Generates a Vec4 : ( 1 ; 1 ; 1 ; 1 ).
				static Vec4 One();

				static Vec4 Lerp(const Vec4& firstVec, const Vec4& secondVec, float ratio);
				static Vec4 NLerp(const Vec4& firstVec, const Vec4& secondVec, float ratio);

				//Operations
				void Homogenize();
				void Normalize();
				Vec4 GetNormalized() const;
				float GetMagnitude() const;
				float GetMagnitudeSQ() const;
				float DotProduct(const Vec4&) const;
				Vec4 CrossProduct(const Vec4&) const;

				//Operators
				Vec4& operator=(const Vec4&);
				Vec4& operator=(Vec4&&) noexcept;
				Vec4 operator+(const Vec4&) const;
				Vec4& operator+=(const Vec4&);
				Vec4 operator-(const Vec4&) const;
				Vec4& operator-=(const Vec4&);
				Vec4 operator*(float) const;
				Vec4& operator*=(float);
				Vec4 operator/(float) const;
				Vec4& operator/=(float);
				bool operator==(const Vec4&) const;
				bool operator!=(const Vec4&) const;
			};

			//External Operators
			std::ostream& operator<<(std::ostream&, const Vec4&);
			Vec4 operator*(float, const Vec4&);
		}
	}
}

#endif
