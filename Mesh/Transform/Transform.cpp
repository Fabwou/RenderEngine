#include "Transform.h"

using namespace engine::core::maths;

Transform::Transform() :
	m_pos{ Vec3::Zero() },
	m_rot{ Quaternion::Identity() },
	m_scale{ Vec3::One() },
	m_worldMat{ Mat4::Identity() }
{}

Transform::Transform(const Transform& copy) :
	m_pos{ copy.m_pos },
	m_rot{ copy.m_rot },
	m_scale{ copy.m_scale },
	m_worldMat{ copy.m_worldMat }
{}

Transform::Transform(Transform&& copy) noexcept :
	m_pos{ copy.m_pos },
	m_rot{ copy.m_rot },
	m_scale{ copy.m_scale },
	m_worldMat{ copy.m_worldMat }
{}

Transform::~Transform()
{}

void Transform::Translate(const Vec3& translation)
{
	m_pos += translation;
	m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);
}

void Transform::Rotate(const Quaternion& rotation)
{
	//rotate on local axis
	m_rot *= rotation;
	m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);

	////rotate around world's origin
	//m_rot = rotation * m_rot;
	//m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);
}

void Transform::Scale(const Vec3& scale)
{
	m_scale += scale;
	m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);
}

Vec3 Transform::GetPosition() const
{
	return m_pos;
}

Quaternion Transform::GetRotation() const
{
	return m_rot;
}

Vec3 Transform::GetScale() const
{
	return m_scale;
}

Mat4 Transform::GetWorldMat() const
{
	return m_worldMat;
}

Vec3 Transform::GetRight() const
{
	return GetRotation().GetRotatedVertex(Vec3::Right());
}

Vec3 Transform::GetUp() const
{
	return GetRotation().GetRotatedVertex(Vec3::Up());
}

Vec3 Transform::GetForward() const
{
	return GetRotation().GetRotatedVertex(Vec3::Forward());
}

void Transform::SetPosition(const Vec3& position)
{
	m_pos = position;
	m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);
}

void Transform::SetRotation(const Quaternion& rotation)
{
	m_rot = rotation;
	m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);
}

void Transform::SetScale(const Vec3& scale)
{
	m_scale = scale;
	m_worldMat = Mat4::TRS(m_pos, m_rot, m_scale);
}

void Transform::SetTRS(const Vec3& pos, const Quaternion& rot, const Vec3& scale) {

	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	m_worldMat = Mat4::TRS(pos, rot, scale);
}

Transform& Transform::operator=(const Transform& other) {

	m_pos = other.m_pos;
	m_rot = other.m_rot;
	m_scale = other.m_scale;
	m_worldMat = other.m_worldMat;

	return *this;
}
Transform& Transform::operator=(Transform&& other) noexcept {

	m_pos = other.m_pos;
	m_rot = other.m_rot;
	m_scale = other.m_scale;
	m_worldMat = other.m_worldMat;

	return *this;
}

bool Transform::operator==(const Transform& other) const {

	return (m_pos == other.m_pos) && (m_rot == other.m_rot) && (m_scale == other.m_scale) && (m_worldMat == other.m_worldMat);
}
bool Transform::operator!=(const Transform& other) const {

	return (m_pos != other.m_pos) || (m_rot != other.m_rot) || (m_scale != other.m_scale) || (m_worldMat != other.m_worldMat);

}