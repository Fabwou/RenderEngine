#include <iomanip>
#include <math.h>

#include "Mat4.h"

#include "../Tools/Defines.h"
#include "../Quaternions/Quaternion.h"

#define AA m_matrix[0][0]
#define AB m_matrix[0][1]
#define AC m_matrix[0][2]
#define AD m_matrix[0][3]
#define BA m_matrix[1][0]
#define BB m_matrix[1][1]
#define BC m_matrix[1][2]
#define BD m_matrix[1][3]
#define CA m_matrix[2][0]
#define CB m_matrix[2][1]
#define CC m_matrix[2][2]
#define CD m_matrix[2][3]
#define DA m_matrix[3][0]
#define DB m_matrix[3][1]
#define DC m_matrix[3][2]
#define DD m_matrix[3][3]

#define SWAP(buffer, component1, component2) \
	buffer = component1; \
	component1 = component2; \
	component2 = buffer

using namespace engine::core::maths;

//Constructors
Mat4::Mat4() :
	m_matrix{
		{ 1.f, 0.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f, 0.f },
		{ 0.f, 0.f, 1.f, 0.f },
		{ 0.f, 0.f, 0.f, 1.f }
}
{}

Mat4::Mat4(const Mat4& other) : 
	m_matrix{
		{ other.AA, other.AB, other.AC, other.AD },
		{ other.BA, other.BB, other.BC, other.BD },
		{ other.CA, other.CB, other.CC, other.CD },
		{ other.DA, other.DB, other.DC, other.DD }
}
{}
Mat4::Mat4(Mat4&& other) noexcept :
	m_matrix{
		{ other.AA, other.AB, other.AC, other.AD },
		{ other.BA, other.BB, other.BC, other.BD },
		{ other.CA, other.CB, other.CC, other.CD },
		{ other.DA, other.DB, other.DC, other.DD }
}
{}

Mat4::Mat4(float aa, float ab, float ac, float ad,
	float ba, float bb, float bc, float bd,
	float ca, float cb, float cc, float cd,
	float da, float db, float dc, float dd) :
	m_matrix{
		{ aa, ab, ac, ad },
		{ ba, bb, bc, bd },
		{ ca, cb, cc, cd },
		{ da, db, dc, dd }
}
{}

Mat4::Mat4(float matrix[4][4]) :
	m_matrix{
		{ matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3] },
		{ matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3] },
		{ matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3] },
		{ matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3] }
}
{}

Mat4::~Mat4()
{}

//Tools
Mat4 Mat4::TRS(const Vec3& translation, const Quaternion& rotation, const Vec3& scale)
{
	return  Mat4::CreateTranslationMatrix(translation) * rotation.GetNormalized().GetRotationMatrix() * Mat4::CreateScaleMatrix(scale);
}

Mat4 Mat4::CreateTranslationMatrix(const Vec3& translation)
{
	return Mat4(
		1.f, 0.f, 0.f, translation.m_x,
		0.f, 1.f, 0.f, translation.m_y,
		0.f, 0.f, 1.f, translation.m_z,
		0.f, 0.f, 0.f, 1.f
	);
}

Mat4 Mat4::CreateRotationMatrix(EAxis axis, float angle)
{
	float angleInRadian{ angle * DEG_TO_RAD_f };
	float c{ cosf(angleInRadian) };
	float s{ sinf(angleInRadian) };

	switch (axis)
	{
	case EAxis::X:
		return Mat4(
			1.f, 0.f, 0.f, 0.f,
			0.f, c, -s, 0.f,
			0.f, s, c, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
		break;
	case EAxis::Y:
		return Mat4(
			c, 0.f, s, 0.f,
			0.f, 1.f, 0.f, 0.f,
			-s, 0.f, c, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
		break;
	case EAxis::Z:
		return Mat4(
			c, -s, 0.f, 0.f,
			s, c, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);
		break;
	default:
		return Mat4();
		break;
	}
}
Mat4 Mat4::CreateRotationMatrix(const Quaternion& rotation) {

	return Mat4(rotation.GetRotationMatrix());

}

Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
{
	return Mat4(
		scale.m_x, 0.f, 0.f, 0.f,
		0.f, scale.m_y, 0.f, 0.f,
		0.f, 0.f, scale.m_z, 0.f,
		0.f, 0.f, 0.f,		 1.f
	);
}

//Shortcuts
Mat4 Mat4::Zero()
{
	return Mat4(
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f
	);
}

Mat4 Mat4::Identity()
{
	return Mat4(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

//Operations
void Mat4::Clear()
{
	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			m_matrix[i][j] = 0.0f;
		}
	}
}

void Mat4::Init()
{
	AA = 1.f; AB = 0.f; AC = 0.f; AD = 0.f;
	BA = 0.f; BB = 1.f; BC = 0.f; BD = 0.f;
	CA = 0.f; CB = 0.f; CC = 1.f; CD = 0.f;
	DA = 0.f; DB = 0.f; DC = 0.f; DD = 1.f;
}

void Mat4::Transpose()
{
	float temp;

	SWAP(temp, AB, BA);
	SWAP(temp, AC, CA);
	SWAP(temp, AD, DA);
	SWAP(temp, BC, CB);
	SWAP(temp, BD, DB);
	SWAP(temp, CD, DC);
}

Mat4 Mat4::GetTranspose() const
{
	return Mat4(
		AA, BA, CA, DA,
		AB, BB, CB, DB,
		AC, BC, CC, DC,
		AD, BD, CD, DD
	);
}

bool Mat4::IsOrthogonal() const
{
#pragma region Defines
#define DOT_PRODUCT(X, Y) \
	((a##X * a##Y) + (b##X * b##Y) + (c##X * c##Y) + (d##X * d##Y))

#define MAGNITUDE_SQ(X) \
	((a##X * a##X) + (b##X * b##X) + (c##X * c##X) + (d##X * d##X))
#pragma endregion

	float aa{ AA }, ba{ BA }, ca{ CA }, da{ DA };
	float ab{ AB }, bb{ BB }, cb{ CB }, db{ DB };
	float ac{ AC }, bc{ BC }, cc{ CC }, dc{ DC };
	float ad{ AD }, bd{ BD }, cd{ CD }, dd{ DD };

	return (
		MAGNITUDE_SQ(a) == 1.f &&
		MAGNITUDE_SQ(b) == 1.f &&
		MAGNITUDE_SQ(c) == 1.f &&
		MAGNITUDE_SQ(d) == 1.f &&
		DOT_PRODUCT(a, b) == 0.f &&
		DOT_PRODUCT(a, c) == 0.f &&
		DOT_PRODUCT(a, d) == 0.f &&
		DOT_PRODUCT(b, c) == 0.f &&
		DOT_PRODUCT(b, d) == 0.f &&
		DOT_PRODUCT(c, d) == 0.f
		);

#pragma region Undefs
#undef MAGNITUDE_SQ
#undef DOT_PRODUCT
#pragma endregion
}

float Mat4::GetDeterminant() const
{
	/*
	===============================================================================================
	DETERMINANT OF A 4x4 MATRIX
	===============================================================================================

	| aa ab ac ad |
	| ba bb bc bd |        | bb bc bd |        | ba bc bd |        | ba bb bd |        | ba bb bc |
	| ca cb cc cd | = aa * | cb cc cd | - ab * | ca cc cd | + ac * | ca cb cd | - ad * | ca cb cc |
	| da db dc dd |        | db dc dd |        | da dc dd |        | da db dd |        | da db dc |

	===============================================================================================
	DETERMINANT OF A 3x3 MATRIX
	===============================================================================================

	| aa ab ac |        | ba bb |        | ad bb |        | ad ba |
	| ad ba bb | = aa * | bd ca | - ab * | bc ca | + ac * | bc bd |
	| bc bd ca |

	===============================================================================================
	DETERMINANT OF A 2x2 MATRIX
	===============================================================================================

	| aa ab |
	| ac ad | = aa * ad - ab * ac

	*/

	float ccddMinusCDDC{ CC * DD - CD * DC }, cbddMinusCDDB{ CB * DD - CD * DB }, cbdcMinusCCDB{ CB * DC - CC * DB };
	float caddMinusCDDA{ CA * DD - CD * DA }, cadcMinusCCDA{ CA * DC - CC * DA }, cadbMinusCBDA{ CA * DB - CB * DA };

	return (
		AA * (BB * ccddMinusCDDC - BC * cbddMinusCDDB + BD * cbdcMinusCCDB) -
		AB * (BA * ccddMinusCDDC - BC * caddMinusCDDA + BD * cadcMinusCCDA) +
		AC * (BA * cbddMinusCDDB - BB * caddMinusCDDA + BD * cadbMinusCBDA) -
		AD * (BA * cbdcMinusCCDB - BB * cadcMinusCCDA + BC * cadbMinusCBDA)
		);
}

bool Mat4::Reverse()
{
	float ccddMinusCDDC{ CC * DD - CD * DC }, bcddMinusBDDC{ BC * DD - BD * DC }, bccdMinusBDCC{ BC * CD - BD * CC };
	float acddMinusADDC{ AC * DD - AD * DC }, accdMinusADCC{ AC * CD - AD * CC }, acbdMinusADBC{ AC * BD - AD * BC };
	float cbddMinusCDDB{ CB * DD - CD * DB }, bbddMinusBDDB{ BB * DD - BD * DB }, bbcdMinusBDCB{ BB * CD - BD * CB };
	float abddMinusADDB{ AB * DD - AD * DB }, abcdMinusADCB{ AB * CD - AD * CB }, abbdMinusADBB{ AB * BD - AD * BB };
	float bbdcMinusBCDB{ BB * DC - BC * DB }, bbccMinusBCCB{ BB * CC - BC * CB }, cbdcMinusCCDB{ CB * DC - CC * DB };
	float abdcMinusACDB{ AB * DC - AC * DB }, abccMinusACCB{ AB * CC - AC * CB }, abbcMinusACBB{ AB * BC - AC * BB };

	Mat4 inverse(
		BB * ccddMinusCDDC - CB * bcddMinusBDDC + DB * bccdMinusBDCC,
		CB * acddMinusADDC - DB * accdMinusADCC - AB * ccddMinusCDDC,
		AB * bcddMinusBDDC - BB * acddMinusADDC + DB * acbdMinusADBC,
		BB * accdMinusADCC - CB * acbdMinusADBC - AB * bccdMinusBDCC,

		CA * bcddMinusBDDC - DA * bccdMinusBDCC - BA * ccddMinusCDDC,
		AA * ccddMinusCDDC - CA * acddMinusADDC + DA * accdMinusADCC,
		BA * acddMinusADDC - DA * acbdMinusADBC - AA * bcddMinusBDDC,
		AA * bccdMinusBDCC - BA * accdMinusADCC + CA * acbdMinusADBC,

		BA * cbddMinusCDDB - CA * bbddMinusBDDB + DA * bbcdMinusBDCB,
		CA * abddMinusADDB - DA * abcdMinusADCB - AA * cbddMinusCDDB,
		AA * bbddMinusBDDB - BA * abddMinusADDB + DA * abbdMinusADBB,
		BA * abcdMinusADCB - CA * abbdMinusADBB - AA * bbcdMinusBDCB,

		CA * bbdcMinusBCDB - DA * bbccMinusBCCB - BA * cbdcMinusCCDB,
		AA * cbdcMinusCCDB - CA * abdcMinusACDB + DA * abccMinusACCB,
		BA * abdcMinusACDB - DA * abbcMinusACBB - AA * bbdcMinusBCDB,
		AA * bbccMinusBCCB - BA * abccMinusACCB + CA * abbcMinusACBB
	);

	float det{ AA * inverse.AA + AB * inverse.BA + AC * inverse.CA + AD * inverse.DA };

	if (det == 0.f)
		return false;

	det = 1.f / det;

	AA = inverse.AA * det; AB = inverse.AB * det; AC = inverse.AC * det; AD = inverse.AD * det;
	BA = inverse.BA * det; BB = inverse.BB * det; BC = inverse.BC * det; BD = inverse.BD * det;
	CA = inverse.CA * det; CB = inverse.CB * det; CC = inverse.CC * det; CD = inverse.CD * det;
	DA = inverse.DA * det; DB = inverse.DB * det; DC = inverse.DC * det; DD = inverse.DD * det;

	return true;
}

Mat4 Mat4::GetInverse(bool* succed) const
{
	float ccddMinusCDDC{ CC * DD - CD * DC }, bcddMinusBDDC{ BC * DD - BD * DC }, bccdMinusBDCC{ BC * CD - BD * CC };
	float acddMinusADDC{ AC * DD - AD * DC }, accdMinusADCC{ AC * CD - AD * CC }, acbdMinusADBC{ AC * BD - AD * BC };
	float cbddMinusCDDB{ CB * DD - CD * DB }, bbddMinusBDDB{ BB * DD - BD * DB }, bbcdMinusBDCB{ BB * CD - BD * CB };
	float abddMinusADDB{ AB * DD - AD * DB }, abcdMinusADCB{ AB * CD - AD * CB }, abbdMinusADBB{ AB * BD - AD * BB };
	float bbdcMinusBCDB{ BB * DC - BC * DB }, bbccMinusBCCB{ BB * CC - BC * CB }, cbdcMinusCCDB{ CB * DC - CC * DB };
	float abdcMinusACDB{ AB * DC - AC * DB }, abccMinusACCB{ AB * CC - AC * CB }, abbcMinusACBB{ AB * BC - AC * BB };

	Mat4 inverse(
		BB * ccddMinusCDDC - CB * bcddMinusBDDC + DB * bccdMinusBDCC,
		CB * acddMinusADDC - DB * accdMinusADCC - AB * ccddMinusCDDC,
		AB * bcddMinusBDDC - BB * acddMinusADDC + DB * acbdMinusADBC,
		BB * accdMinusADCC - CB * acbdMinusADBC - AB * bccdMinusBDCC,

		CA * bcddMinusBDDC - DA * bccdMinusBDCC - BA * ccddMinusCDDC,
		AA * ccddMinusCDDC - CA * acddMinusADDC + DA * accdMinusADCC,
		BA * acddMinusADDC - DA * acbdMinusADBC - AA * bcddMinusBDDC,
		AA * bccdMinusBDCC - BA * accdMinusADCC + CA * acbdMinusADBC,

		BA * cbddMinusCDDB - CA * bbddMinusBDDB + DA * bbcdMinusBDCB,
		CA * abddMinusADDB - DA * abcdMinusADCB - AA * cbddMinusCDDB,
		AA * bbddMinusBDDB - BA * abddMinusADDB + DA * abbdMinusADBB,
		BA * abcdMinusADCB - CA * abbdMinusADBB - AA * bbcdMinusBDCB,

		CA * bbdcMinusBCDB - DA * bbccMinusBCCB - BA * cbdcMinusCCDB,
		AA * cbdcMinusCCDB - CA * abdcMinusACDB + DA * abccMinusACCB,
		BA * abdcMinusACDB - DA * abbcMinusACBB - AA * bbdcMinusBCDB,
		AA * bbccMinusBCCB - BA * abccMinusACCB + CA * abbcMinusACBB
	);

	float det{ AA * inverse.AA + AB * inverse.BA + AC * inverse.CA + AD * inverse.DA };

	if (det == 0.f)
	{
		if (succed)
			*succed = false;

		return *this;
	}

	if (succed)
		*succed = true;

	det = 1.f / det;

	inverse.AA *= det; inverse.AB *= det; inverse.AC *= det; inverse.AD *= det;
	inverse.BA *= det; inverse.BB *= det; inverse.BC *= det; inverse.BD *= det;
	inverse.CA *= det; inverse.CB *= det; inverse.CC *= det; inverse.CD *= det;
	inverse.DA *= det; inverse.DB *= det; inverse.DC *= det; inverse.DD *= det;

	return inverse;
}

bool Mat4::GetInverseByRef(Mat4& out) const
{
	float ccddMinusCDDC{ CC * DD - CD * DC }, bcddMinusBDDC{ BC * DD - BD * DC }, bccdMinusBDCC{ BC * CD - BD * CC };
	float acddMinusADDC{ AC * DD - AD * DC }, accdMinusADCC{ AC * CD - AD * CC }, acbdMinusADBC{ AC * BD - AD * BC };
	float cbddMinusCDDB{ CB * DD - CD * DB }, bbddMinusBDDB{ BB * DD - BD * DB }, bbcdMinusBDCB{ BB * CD - BD * CB };
	float abddMinusADDB{ AB * DD - AD * DB }, abcdMinusADCB{ AB * CD - AD * CB }, abbdMinusADBB{ AB * BD - AD * BB };
	float bbdcMinusBCDB{ BB * DC - BC * DB }, bbccMinusBCCB{ BB * CC - BC * CB }, cbdcMinusCCDB{ CB * DC - CC * DB };
	float abdcMinusACDB{ AB * DC - AC * DB }, abccMinusACCB{ AB * CC - AC * CB }, abbcMinusACBB{ AB * BC - AC * BB };

	out.AA = BB * ccddMinusCDDC - CB * bcddMinusBDDC + DB * bccdMinusBDCC;
	out.AB = CB * acddMinusADDC - DB * accdMinusADCC - AB * ccddMinusCDDC;
	out.AC = AB * bcddMinusBDDC - BB * acddMinusADDC + DB * acbdMinusADBC;
	out.AD = BB * accdMinusADCC - CB * acbdMinusADBC - AB * bccdMinusBDCC;

	out.BA = CA * bcddMinusBDDC - DA * bccdMinusBDCC - BA * ccddMinusCDDC;
	out.BB = AA * ccddMinusCDDC - CA * acddMinusADDC + DA * accdMinusADCC;
	out.BC = BA * acddMinusADDC - DA * acbdMinusADBC - AA * bcddMinusBDDC;
	out.BD = AA * bccdMinusBDCC - BA * accdMinusADCC + CA * acbdMinusADBC;

	out.CA = BA * cbddMinusCDDB - CA * bbddMinusBDDB + DA * bbcdMinusBDCB;
	out.CB = CA * abddMinusADDB - DA * abcdMinusADCB - AA * cbddMinusCDDB;
	out.CC = AA * bbddMinusBDDB - BA * abddMinusADDB + DA * abbdMinusADBB;
	out.CD = BA * abcdMinusADCB - CA * abbdMinusADBB - AA * bbcdMinusBDCB;

	out.DA = CA * bbdcMinusBCDB - DA * bbccMinusBCCB - BA * cbdcMinusCCDB;
	out.DB = AA * cbdcMinusCCDB - CA * abdcMinusACDB + DA * abccMinusACCB;
	out.DC = BA * abdcMinusACDB - DA * abbcMinusACBB - AA * bbdcMinusBCDB;
	out.DD = AA * bbccMinusBCCB - BA * abccMinusACCB + CA * abbcMinusACBB;

	float det{ AA * out.AA + AB * out.BA + AC * out.CA + AD * out.DA };

	if (det == 0.f)
		return false;

	det = 1.f / det;

	out.AA *= det; out.AB *= det; out.AC *= det; out.AD *= det;
	out.BA *= det; out.BB *= det; out.BC *= det; out.BD *= det;
	out.CA *= det; out.CB *= det; out.CC *= det; out.CD *= det;
	out.DA *= det; out.DB *= det; out.DC *= det; out.DD *= det;

	return true;
}

//Operators
Mat4& Mat4::operator=(const Mat4& mat) {

	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			m_matrix[i][j] = mat.m_matrix[i][j];
		}
	}
	return *this;
};
Mat4& Mat4::operator=(Mat4&& mat) noexcept {

	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			m_matrix[i][j] = mat.m_matrix[i][j];
		}
	}
	return *this;
};

Mat4 Mat4::operator+(const Mat4& mat) const
{
#define SUM(row, column) \
	(m_matrix[row][column] + mat.m_matrix[row][column])

	return Mat4(
		SUM(0, 0), SUM(0, 1), SUM(0, 2), SUM(0, 3),
		SUM(1, 0), SUM(1, 1), SUM(1, 2), SUM(1, 3),
		SUM(2, 0), SUM(2, 1), SUM(2, 2), SUM(2, 3),
		SUM(3, 0), SUM(3, 1), SUM(3, 2), SUM(3, 3)
	);

#undef SUM
}

Mat4& Mat4::operator+=(const Mat4& mat)
{
#define ADD(row, column) \
	m_matrix[row][column] += mat.m_matrix[row][column]

	ADD(0, 0); ADD(0, 1); ADD(0, 2); ADD(0, 3);
	ADD(1, 0); ADD(1, 1); ADD(1, 2); ADD(1, 3);
	ADD(2, 0); ADD(2, 1); ADD(2, 2); ADD(2, 3);
	ADD(3, 0); ADD(3, 1); ADD(3, 2); ADD(3, 3);

	return *this;

#undef ADD
}

Mat4 Mat4::operator-() const
{
	return Mat4(
		-AA, -AB, -AC, -AD,
		-BA, -BB, -BC, -BD,
		-CA, -CB, -CC, -CD,
		-DA, -DB, -DC, -DD
	);
}

Mat4 Mat4::operator-(const Mat4& mat) const
{
#define DIFF(component) \
	(component - mat.component)

	return Mat4(
		DIFF(AA), DIFF(AB), DIFF(AC), DIFF(AD),
		DIFF(BA), DIFF(BB), DIFF(BC), DIFF(BD),
		DIFF(CA), DIFF(CB), DIFF(CC), DIFF(CD),
		DIFF(DA), DIFF(DB), DIFF(DC), DIFF(DD)
	);

#undef DIFF
}

Mat4& Mat4::operator-=(const Mat4& mat)
{
#define SUB(component) \
	component -= mat.component

	SUB(AA); SUB(AB); SUB(AC); SUB(AD);
	SUB(BA); SUB(BB); SUB(BC); SUB(BD);
	SUB(CA); SUB(CB); SUB(CC); SUB(CD);
	SUB(DA); SUB(DB); SUB(DC); SUB(DD);

	return *this;

#undef SUB
}

Mat4 Mat4::operator*(float scalar) const
{
#define PRODUCT(component) \
	(component * scalar)

	return Mat4(
		PRODUCT(AA), PRODUCT(AB), PRODUCT(AC), PRODUCT(AD),
		PRODUCT(BA), PRODUCT(BB), PRODUCT(BC), PRODUCT(BD),
		PRODUCT(CA), PRODUCT(CB), PRODUCT(CC), PRODUCT(CD),
		PRODUCT(DA), PRODUCT(DB), PRODUCT(DC), PRODUCT(DD)
	);

#undef PRODUCT
}

Mat4 Mat4::operator*(const Mat4& mat) const
{
#define DOT_PRODUCT(row, column) \
	(m_matrix[row][0] * mat.m_matrix[0][column] + m_matrix[row][1] * mat.m_matrix[1][column] + m_matrix[row][2] * mat.m_matrix[2][column] + m_matrix[row][3] * mat.m_matrix[3][column])

	return Mat4(
		DOT_PRODUCT(0, 0), DOT_PRODUCT(0, 1), DOT_PRODUCT(0, 2), DOT_PRODUCT(0, 3),
		DOT_PRODUCT(1, 0), DOT_PRODUCT(1, 1), DOT_PRODUCT(1, 2), DOT_PRODUCT(1, 3),
		DOT_PRODUCT(2, 0), DOT_PRODUCT(2, 1), DOT_PRODUCT(2, 2), DOT_PRODUCT(2, 3),
		DOT_PRODUCT(3, 0), DOT_PRODUCT(3, 1), DOT_PRODUCT(3, 2), DOT_PRODUCT(3, 3)
	);

#undef DOT_PRODUCT
}

Vec4 Mat4::operator*(const Vec4& vec) const
{
#define DOT_PRODUCT_VEC(row) \
	(m_matrix[row][0] * vec.m_x + m_matrix[row][1] * vec.m_y + m_matrix[row][2] * vec.m_z + m_matrix[row][3] * vec.m_w)

	return Vec4(
		DOT_PRODUCT_VEC(0),
		DOT_PRODUCT_VEC(1),
		DOT_PRODUCT_VEC(2),
		DOT_PRODUCT_VEC(3)
	);

#undef DOT_PRODUCT_VEC
}

Mat4& Mat4::operator*=(float scalar)
{
#define MULT(component) \
	component *= scalar

	MULT(AA); MULT(AB); MULT(AC); MULT(AD);
	MULT(BA); MULT(BB); MULT(BC); MULT(BD);
	MULT(CA); MULT(CB); MULT(CC); MULT(CD);
	MULT(DA); MULT(DB); MULT(DC); MULT(DD);

	return *this;

#undef MULT
}

Mat4& Mat4::operator*=(const Mat4& mat)
{
#define DOT_PRODUCT(row, column) \
	(m_matrix[row][0] * mat.m_matrix[0][column] + m_matrix[row][1] * mat.m_matrix[1][column] + m_matrix[row][2] * mat.m_matrix[2][column] + m_matrix[row][3] * mat.m_matrix[3][column])

	float aa{ DOT_PRODUCT(0, 0) }, ab{ DOT_PRODUCT(0, 1) }, ac{ DOT_PRODUCT(0, 2) }, ad{ DOT_PRODUCT(0, 3) };
	float ba{ DOT_PRODUCT(1, 0) }, bb{ DOT_PRODUCT(1, 1) }, bc{ DOT_PRODUCT(1, 2) }, bd{ DOT_PRODUCT(1, 3) };
	float ca{ DOT_PRODUCT(2, 0) }, cb{ DOT_PRODUCT(2, 1) }, cc{ DOT_PRODUCT(2, 2) }, cd{ DOT_PRODUCT(2, 3) };
	float da{ DOT_PRODUCT(3, 0) }, db{ DOT_PRODUCT(3, 1) }, dc{ DOT_PRODUCT(3, 2) }, dd{ DOT_PRODUCT(3, 3) };

	AA = aa; AB = ab; AC = ac; AD = ad;
	BA = ba; BB = bb; BC = bc; BD = bd;
	CA = ca; CB = cb; CC = cc; CD = cd;
	DA = da; DB = db; DC = dc; DD = dd;

	return *this;

#undef DOT_PRODUCT
}

bool Mat4::operator==(const Mat4& mat) const
{
#pragma region Defines
#define SAME_VALUE(row, column) \
	(m_matrix[row][column] == mat.m_matrix[row][column])

#define SAME_ROW(row) \
	(SAME_VALUE(row, 0) && SAME_VALUE(row, 1) && SAME_VALUE(row, 2) && SAME_VALUE(row, 3))

#define SAME() \
	(SAME_ROW(0) && SAME_ROW(1) && SAME_ROW(2) && SAME_ROW(3))
#pragma endregion

	return SAME();

#pragma region Undefs
#undef SAME
#undef SAME_ROW
#undef SAME_VALUE
#pragma endregion
}

bool Mat4::operator!=(const Mat4& mat) const
{
#pragma region Defines
#define SAME_VALUE(row, column) \
	(m_matrix[row][column] == mat.m_matrix[row][column])

#define SAME_ROW(row) \
	(SAME_VALUE(row, 0) && SAME_VALUE(row, 1) && SAME_VALUE(row, 2) && SAME_VALUE(row, 3))

#define SAME() \
	(SAME_ROW(0) && SAME_ROW(1) && SAME_ROW(2) && SAME_ROW(3))
#pragma endregion

	return !SAME();

#pragma region Undefs
#undef SAME
#undef SAME_ROW
#undef SAME_VALUE
#pragma endregion
}

float* Mat4::operator[](int index)
{
	return m_matrix[index];
}

const float* Mat4::operator[](int index) const
{
	return m_matrix[index];
}

//External Operators
std::ostream& engine::core::maths::operator<<(std::ostream& stream, const Mat4& mat)
{
	std::cout << std::setprecision(2) << std::fixed;
	stream << "Mat4 :\n(";
	for (int row{ 0 }; row < 4; ++row)
	{
		for (int column{ 0 }; column < 4; ++column)
			stream << "	" << mat.m_matrix[row][column];

		if (row == 3)
			stream << "	)";

		stream << std::endl;
	}
	std::cout << std::setprecision(0) << std::defaultfloat;

	return stream;
}

Mat4 engine::core::maths::operator*(float scalar, const Mat4& mat)
{
#define PRODUCT(component) \
	(mat.component * scalar)

	return Mat4(
		PRODUCT(AA), PRODUCT(AB), PRODUCT(AC), PRODUCT(AD),
		PRODUCT(BA), PRODUCT(BB), PRODUCT(BC), PRODUCT(BD),
		PRODUCT(CA), PRODUCT(CB), PRODUCT(CC), PRODUCT(CD),
		PRODUCT(DA), PRODUCT(DB), PRODUCT(DC), PRODUCT(DD)
	);

#undef PRODUCT
}