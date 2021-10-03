#include <iomanip>
#include "Vec2.h"
#include "../../Tools/Defines.h"

using namespace engine::core::maths;

//Constructors/Destructors
Vec2::Vec2() :
	m_x{ 0.f },
	m_y{ 0.f }
{}

Vec2::Vec2(const Vec2& lVec2) :
	m_x{ lVec2.m_x },
	m_y{ lVec2.m_y }
{}

Vec2::Vec2(Vec2&& rVec2) noexcept :
	m_x{ rVec2.m_x },
	m_y{ rVec2.m_y }
{}

Vec2::Vec2(float x, float y) :
	m_x{ x },
	m_y{ y }
{}

Vec2::~Vec2()
{

}

//Shortcuts
Vec2 Vec2::Zero()
{
	return Vec2(0.f, 0.f);
}

Vec2 Vec2::Left()
{
	return Vec2(-1.f, 0.f);
}

Vec2 Vec2::Right()
{
	return Vec2(1.f, 0.f);
}

Vec2 Vec2::Down()
{
	return Vec2(0.f, -1.f);
}

Vec2 Vec2::Up()
{
	return Vec2(0.f, 1.f);
}

Vec2 Vec2::One()
{
	return Vec2(1.f, 1.f);
}

Vec2 engine::core::maths::Vec2::Lerp(const Vec2& firstVec, const Vec2& secondVec, float ratio)
{
	ratio = CLAMP(ratio, 0.f, 1.f);

	return Vec2(
		LERP(firstVec.m_x, secondVec.m_x, ratio),
		LERP(firstVec.m_y, secondVec.m_y, ratio)
	);
}

Vec2 engine::core::maths::Vec2::NLerp(const Vec2& firstVec, const Vec2& secondVec, float ratio)
{
	ratio = CLAMP(ratio, 0.f, 1.f);

	Vec2 result(
		LERP(firstVec.m_x, secondVec.m_x, ratio),
		LERP(firstVec.m_y, secondVec.m_y, ratio)
	);

	result.Normalize();

	return result;
}

bool Vec2::CompareWithEpsilon(Vec2 a, Vec2 b, float epsilon) {

	return ((abs(a.m_x - b.m_x) <= epsilon) &&
		(abs(a.m_y - b.m_y) <= epsilon));
}


//Operations
void Vec2::Normalize()
{
	if (m_x != 0.f || m_y != 0.f)
	{
		float magnitude{ GetMagnitude() };

		m_x /= magnitude;
		m_y /= magnitude;
	}
}

Vec2 Vec2::GetNormalized() const
{
	if (m_x == 0.f && m_y == 0.f)
		return *this;

	else
	{
		float magnitude{ GetMagnitude() };

		return Vec2(
			m_x / magnitude,
			m_y / magnitude
		);
	}
}

float Vec2::GetMagnitude() const
{
	return (m_x * m_x) + (m_y * m_y);
}

float Vec2::GetMagnitudeSQ() const
{
	return sqrtf((m_x * m_x) + (m_y * m_y));
}

float Vec2::DotProduct(const Vec2& vec) const
{
	return (m_x * vec.m_x) + (m_y * vec.m_y);
}

//Operators
Vec2& Vec2::operator=(const Vec2& vec)
{
	m_x = vec.m_x;
	m_y = vec.m_y;

	return *this;
}

Vec2& Vec2::operator=(Vec2&& vec) noexcept
{
	m_x = vec.m_x;
	m_y = vec.m_y;

	return *this;
}

Vec2 Vec2::operator+(const Vec2& vec) const
{
	return Vec2(
		m_x + vec.m_x,
		m_y + vec.m_y
	);
}

Vec2& Vec2::operator+=(const Vec2& vec)
{
	m_x += vec.m_x;
	m_y += vec.m_y;

	return *this;
}

Vec2 Vec2::operator-(const Vec2& vec) const
{
	return Vec2(
		m_x - vec.m_x,
		m_y - vec.m_y
	);
}

Vec2& Vec2::operator-=(const Vec2& vec)
{
	m_x -= vec.m_x;
	m_y -= vec.m_y;

	return *this;
}

Vec2 Vec2::operator*(float factor) const
{
	return Vec2(
		m_x * factor,
		m_y * factor
	);
}

Vec2& Vec2::operator*=(float factor)
{
	m_x *= factor;
	m_y *= factor;

	return *this;
}

Vec2 Vec2::operator/(float divisor) const
{
	if (divisor == 0.f)
		return Vec2::Zero();

	else
		return Vec2(
			m_x / divisor,
			m_y / divisor
		);
}

Vec2& Vec2::operator/=(float divisor)
{
	if (divisor == 0.f)
	{
		m_x = 0.f;
		m_y = 0.f;
	}

	else
	{
		m_x /= divisor;
		m_y /= divisor;
	}

	return *this;
}

bool Vec2::operator==(const Vec2& vec) const
{
	return (m_x == vec.m_x) && (m_y == vec.m_y);
}

bool Vec2::operator!=(const Vec2& vec) const
{
	return (m_x != vec.m_x) || (m_y != vec.m_y);
}

//External Operators

std::ostream& engine::core::maths::operator<<(std::ostream& out, const Vec2& vec)
{
	std::cout << std::setprecision(2) << std::fixed;
	out << "Vec2( " << vec.m_x << " ; " << vec.m_y << " )";
	//std::cout << std::setprecision(0) << std::defaultfloat;

	return out;
}

Vec2 engine::core::maths::operator*(float factor, const Vec2& vec)
{
	return Vec2(
		vec.m_x * factor,
		vec.m_y * factor
	);
}