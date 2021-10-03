#include <iomanip>

#include "Vec3.h"
#include "../../Tools/Defines.h"

using namespace engine::core::maths;

//Constructors/Destructors
Vec3::Vec3() :
	m_x{ 0.f },
	m_y{ 0.f },
	m_z{ 0.f }
{}

Vec3::Vec3(float x, float y, float z) :
	m_x{ x },
	m_y{ y },
	m_z{ z }
{}

Vec3::Vec3(const Vec3& lVec3) :
	m_x{ lVec3.m_x },
	m_y{ lVec3.m_y },
	m_z{ lVec3.m_z }
{}

Vec3::Vec3(Vec3&& rVec3) noexcept :
	m_x{ rVec3.m_x },
	m_y{ rVec3.m_y },
	m_z{ rVec3.m_z }
{}

Vec3::~Vec3()
{}

//Shortcuts
Vec3 Vec3::Zero()
{
	return Vec3(0.f, 0.f, 0.f);
}

Vec3 Vec3::Left()
{
	return Vec3(-1.f, 0.f, 0.f);
}

Vec3 Vec3::Right()
{
	return Vec3(1.f, 0.f, 0.f);
}

Vec3 Vec3::Down()
{
	return Vec3(0.f, -1.f, 0.f);
}

Vec3 Vec3::Up()
{
	return Vec3(0.f, 1.f, 0.f);
}

Vec3 Vec3::Back()
{
	return Vec3(0.f, 0.f, -1.f);
}

Vec3 Vec3::Forward()
{
	return Vec3(0.f, 0.f, 1.f);
}

Vec3 Vec3::One()
{
	return Vec3(1.f, 1.f, 1.f);
}

Vec3 engine::core::maths::Vec3::Lerp(const Vec3& firstVec, const Vec3& secondVec, float ratio)
{
	ratio = CLAMP(ratio, 0.f, 1.f);

	return Vec3(
		LERP(firstVec.m_x, secondVec.m_x, ratio),
		LERP(firstVec.m_y, secondVec.m_y, ratio),
		LERP(firstVec.m_z, secondVec.m_z, ratio)
	);
}

Vec3 engine::core::maths::Vec3::NLerp(const Vec3& firstVec, const Vec3& secondVec, float ratio)
{
	ratio = CLAMP(ratio, 0.f, 1.f);

	Vec3 result(
		LERP(firstVec.m_x, secondVec.m_x, ratio),
		LERP(firstVec.m_y, secondVec.m_y, ratio),
		LERP(firstVec.m_z, secondVec.m_z, ratio)
	);

	result.Normalize();

	return result;
}

bool Vec3::CompareWithEpsilon(Vec3 a, Vec3 b, float epsilon) {

	return ((abs(a.m_x - b.m_x) <= epsilon) &&
		(abs(a.m_y - b.m_y) <= epsilon) &&
		(abs(a.m_z - b.m_z) <= epsilon));
}


//Operations
void Vec3::Normalize()
{
	if (m_x != 0.f || m_y != 0.f || m_z != 0.f)
	{
		float magnitude{ GetMagnitude() };

		m_x /= magnitude;
		m_y /= magnitude;
		m_z /= magnitude;
	}
}

Vec3 Vec3::GetNormalized() const
{
	if (m_x == 0.f && m_y == 0.f && m_z == 0.f)
		return *this;

	else
	{
		float magnitude{ GetMagnitude() };

		return Vec3(
			m_x / magnitude,
			m_y / magnitude,
			m_z / magnitude
		);
	}
}

float Vec3::GetMagnitude() const
{
	return sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

float Vec3::GetMagnitudeSQ() const
{
	return (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
}

float Vec3::DotProduct(const Vec3& vec) const
{
	return ((m_x * vec.m_x) + (m_y * vec.m_y) + (m_z * vec.m_z));
}

Vec3 Vec3::CrossProduct(const Vec3& vec) const
{
	return Vec3(
		(m_y * vec.m_z) - (m_z * vec.m_y),
		(m_z * vec.m_x) - (m_x * vec.m_z),
		(m_x * vec.m_y) - (m_y * vec.m_x)
	);
}

//Operators
Vec3& Vec3::operator=(const Vec3& vec)
{
	m_x = vec.m_x;
	m_y = vec.m_y;
	m_z = vec.m_z;

	return *this;
}

Vec3& Vec3::operator=(Vec3&& vec) noexcept
{
	m_x = vec.m_x;
	m_y = vec.m_y;
	m_z = vec.m_z;

	return *this;
}

Vec3 Vec3::operator+(const Vec3& vec) const
{
	return Vec3(
		m_x + vec.m_x,
		m_y + vec.m_y,
		m_z + vec.m_z
	);
}

Vec3& Vec3::operator+=(const Vec3& vec)
{
	m_x += vec.m_x;
	m_y += vec.m_y;
	m_z += vec.m_z;

	return *this;
}

Vec3 Vec3::operator-(const Vec3& vec) const
{
	return Vec3(
		m_x - vec.m_x,
		m_y - vec.m_y,
		m_z - vec.m_z
	);
}

Vec3& Vec3::operator-=(const Vec3& vec)
{
	m_x -= vec.m_x;
	m_y -= vec.m_y;
	m_z -= vec.m_z;

	return *this;
}

Vec3 Vec3::operator*(float factor) const
{
	return Vec3(
		m_x * factor,
		m_y * factor,
		m_z * factor
	);
}

Vec3& Vec3::operator*=(float factor)
{
	m_x *= factor;
	m_y *= factor;
	m_z *= factor;

	return *this;
}

Vec3 Vec3::operator/(float divisor) const
{
	if (divisor == 0.f)
		return Vec3::Zero();

	else
		return Vec3(
			m_x / divisor,
			m_y / divisor,
			m_z / divisor
		);
}

Vec3& Vec3::operator/=(float divisor)
{
	if (divisor == 0.f)
	{
		m_x = 0.f;
		m_y = 0.f;
		m_z = 0.f;
	}

	else
	{
		m_x /= divisor;
		m_y /= divisor;
		m_z /= divisor;
	}

	return *this;
}

bool Vec3::operator==(const Vec3& vec) const
{
	return (m_x == vec.m_x) && (m_y == vec.m_y) && (m_z == vec.m_z);
}

bool Vec3::operator!=(const Vec3& vec) const
{
	return (m_x != vec.m_x) || (m_y != vec.m_y) || (m_z != vec.m_z);
}

//External Operators
std::ostream& engine::core::maths::operator<<(std::ostream& out, const Vec3& vec)
{
	std::cout << std::setprecision(2) << std::fixed;
	out << "Vec3( " << vec.m_x << " ; " << vec.m_y << " ; " << vec.m_z << " )";
	std::cout << std::setprecision(0) << std::defaultfloat;

	return out;
}

Vec3 engine::core::maths::operator*(float factor, const Vec3& vec)
{
	return Vec3(
		vec.m_x * factor,
		vec.m_y * factor,
		vec.m_z * factor
	);
}