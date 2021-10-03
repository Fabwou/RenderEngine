#ifndef _VEC2_H_
#define _VEC2_H_

#include <iostream>

namespace engine {
	namespace core {
		namespace maths {

			struct Vec2
			{
				//Components
				float m_x;
				float m_y;

				//Constructors/Destructors
				Vec2();
				Vec2(const Vec2& lVec2);
				Vec2(Vec2 && rVec2) noexcept;
				Vec2(float x, float y);
				~Vec2();

				//Shortcuts
				// Generates a Vec2 : ( 0 ; 0 ).
				static Vec2 Zero();
				// Generates a Vec2 : ( -1 ; 0 ).
				static Vec2 Left();
				// Generates a Vec2 : ( 1 ; 0 ).
				static Vec2 Right();
				// Generates a Vec2 : ( 0 ; -1 ).
				static Vec2 Down();
				// Generates a Vec2 : ( 0 ; 1 ).
				static Vec2 Up();
				// Generates a Vec2 : ( 1 ; 1 ).
				static Vec2 One();

				static Vec2 Lerp(const Vec2& firstVec, const Vec2& secondVec, float ratio);
				static Vec2 NLerp(const Vec2& firstVec, const Vec2& secondVec, float ratio);
				static bool CompareWithEpsilon(Vec2 a, Vec2 b, float epsilon = 0.00001f);

				//Operations
				void Normalize();
				Vec2 GetNormalized() const;
				float GetMagnitude() const;
				float GetMagnitudeSQ() const;
				float DotProduct(const Vec2&) const;

				//Operators
				Vec2& operator=(const Vec2&);
				Vec2& operator=(Vec2&&) noexcept;
				Vec2 operator+(const Vec2&) const;
				Vec2& operator+=(const Vec2&);
				Vec2 operator-(const Vec2&) const;
				Vec2& operator-=(const Vec2&);
				Vec2 operator*(float) const;
				Vec2& operator*=(float);
				Vec2 operator/(float) const;
				Vec2& operator/=(float);
				bool operator==(const Vec2&) const;
				bool operator!=(const Vec2&) const;
			};

			//External Operators
			std::ostream& operator<<(std::ostream&, const Vec2&);
			Vec2 operator*(float, const Vec2&);

		}
	}
}

#endif