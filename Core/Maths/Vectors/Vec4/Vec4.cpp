#include <iomanip>

#include "Vec4.h"
#include "../../Tools/Defines.h"

using namespace engine::core::maths;

//Constructors/Destructors
Vec4::Vec4() :
	m_x{ 0.f },
	m_y{ 0.f },
	m_z{ 0.f },
	m_w{ 1.f }
{}
Vec4::Vec4(const Vec4& lVec4) :
	m_x{ lVec4.m_x },
	m_y{ lVec4.m_y },
	m_z{ lVec4.m_z },
	m_w{ lVec4.m_w }
{}
Vec4::Vec4(Vec4&& rVec4) noexcept :
	m_x{ rVec4.m_x },
	m_y{ rVec4.m_y },
	m_z{ rVec4.m_z },
	m_w{ rVec4.m_w }
{};

Vec4::Vec4(float x, float y, float z, float w) :
	m_x{ x },
	m_y{ y },
	m_z{ z },
	m_w{ w }
{}

Vec4::~Vec4()
{}

//Shortcuts
Vec4 Vec4::Zero()
{
	return Vec4(0.f, 0.f, 0.f, 1.f);
}

Vec4 Vec4::Left()
{
	return Vec4(-1.f, 0.f, 0.f, 1.f);
}

Vec4 Vec4::Right()
{
	return Vec4(1.f, 0.f, 0.f, 1.f);
}

Vec4 Vec4::Down()
{
	return Vec4(0.f, -1.f, 0.f, 1.f);
}

Vec4 Vec4::Up()
{
	return Vec4(0.f, 1.f, 0.f, 1.f);
}

Vec4 Vec4::Back()
{
	return Vec4(0.f, 0.f, -1.f, 1.f);
}

Vec4 Vec4::Forward()
{
	return Vec4(0.f, 0.f, 1.f, 1.f);
}

Vec4 Vec4::One()
{
	return Vec4(1.f, 1.f, 1.f, 1.f);
}

Vec4 engine::core::maths::Vec4::Lerp(const Vec4& firstVec, const Vec4& secondVec, float ratio)
{
	ratio = CLAMP(ratio, 0.f, 1.f);

	return Vec4(
		LERP(firstVec.m_x, secondVec.m_x, ratio),
		LERP(firstVec.m_y, secondVec.m_y, ratio),
		LERP(firstVec.m_z, secondVec.m_z, ratio)
	);
}

Vec4 engine::core::maths::Vec4::NLerp(const Vec4& firstVec, const Vec4& secondVec, float ratio)
{
	ratio = CLAMP(ratio, 0.f, 1.f);

	Vec4 result(
		LERP(firstVec.m_x, secondVec.m_x, ratio),
		LERP(firstVec.m_y, secondVec.m_y, ratio),
		LERP(firstVec.m_z, secondVec.m_z, ratio)
	);

	result.Normalize();

	return result;
}

//Operations
void Vec4::Homogenize()
{
	if (m_w != 0.f)
	{
		m_x /= m_w;
		m_y /= m_w;
		m_z /= m_w;
	}
}

void Vec4::Normalize()
{
	if (m_x != 0.f || m_y != 0.f || m_z != 0.f)
	{
		float magnitude{ GetMagnitude() };

		m_x /= magnitude;
		m_y /= magnitude;
		m_z /= magnitude;
	}
}

Vec4 Vec4::GetNormalized() const
{
	if (m_x == 0.f && m_y == 0.f && m_z == 0.f)
		return *this;

	float magnitude{ GetMagnitude() };

	return Vec4(
		m_x / magnitude,
		m_y / magnitude,
		m_z / magnitude,
		m_w
	);
}

float Vec4::GetMagnitude() const
{
	return sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
}

float Vec4::GetMagnitudeSQ() const
{
	return (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
}

float Vec4::DotProduct(const Vec4& vec) const
{
	return ((m_x * vec.m_x) + (m_y * vec.m_y) + (m_z * vec.m_z));
}

Vec4 Vec4::CrossProduct(const Vec4& vec) const
{
	return Vec4(
		(m_y * vec.m_z) - (m_z * vec.m_y),
		(m_z * vec.m_x) - (m_x * vec.m_z),
		(m_x * vec.m_y) - (m_y * vec.m_x)
	);
}

//Operators
Vec4& Vec4::operator=(const Vec4& vec)
{
	m_x = vec.m_x;
	m_y = vec.m_y;
	m_z = vec.m_z;
	m_w = vec.m_w;

	return *this;
}

Vec4& Vec4::operator=(Vec4&& vec) noexcept
{
	m_x = vec.m_x;
	m_y = vec.m_y;
	m_z = vec.m_z;
	m_w = vec.m_w;

	return *this;
}
Vec4 Vec4::operator+(const Vec4& vec) const
{
	return Vec4(
		m_x + vec.m_x,
		m_y + vec.m_y,
		m_z + vec.m_z
	);
}

Vec4& Vec4::operator+=(const Vec4& vec)
{
	m_x += vec.m_x;
	m_y += vec.m_y;
	m_z += vec.m_z;

	return *this;
}

Vec4 Vec4::operator-(const Vec4& vec) const
{
	return Vec4(
		m_x - vec.m_x,
		m_y - vec.m_y,
		m_z - vec.m_z
	);
}

Vec4& Vec4::operator-=(const Vec4& vec)
{
	m_x -= vec.m_x;
	m_y -= vec.m_y;
	m_z -= vec.m_z;

	return *this;
}

Vec4 Vec4::operator*(float factor) const
{
	return Vec4(
		m_x * factor,
		m_y * factor,
		m_z * factor,
		m_w
	);
}

Vec4& Vec4::operator*=(float factor)
{
	m_x *= factor;
	m_y *= factor;
	m_z *= factor;

	return *this;
}

Vec4 Vec4::operator/(float divisor) const
{
	if (divisor == 0.f)
		return Vec4::Zero();

	else
		return Vec4(
			m_x / divisor,
			m_y / divisor,
			m_z / divisor,
			m_w
		);
}

Vec4& Vec4::operator/=(float divisor)
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

bool Vec4::operator==(const Vec4& vec) const
{
	return (m_x == vec.m_x) && (m_y == vec.m_y) && (m_z == vec.m_z) && (m_w == vec.m_w);
}

bool Vec4::operator!=(const Vec4& vec) const
{
	return (m_x != vec.m_x) || (m_y != vec.m_y) || (m_z != vec.m_z) || (m_w != vec.m_w);
}

//External Operators
std::ostream& engine::core::maths::operator<<(std::ostream& out, const Vec4& vec)
{
	std::cout << std::setprecision(2) << std::fixed;
	out << "Vec4( " << vec.m_x << " ; " << vec.m_y << " ; " << vec.m_z << " ; " << vec.m_w << " )";
	std::cout << std::setprecision(0) << std::defaultfloat;

	return out;
}

Vec4 engine::core::maths::operator*(float factor, const Vec4& vec)
{
	return Vec4(
		vec.m_x * factor,
		vec.m_y * factor,
		vec.m_z * factor,
		vec.m_w
	);
}