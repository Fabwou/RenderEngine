#include "Camera.h"
#include "../Core/Maths/Tools/Defines.h"

using namespace engine::core::maths;
using namespace engine::camera;

Camera::Camera(EProjectType type, float width, float height, float _near, float _far, float fov, engine::core::maths::Vec3 pos) :
	m_type{ type },
	m_viewWidth{ width },
	m_viewHeight{ height },
	m_fov{ fov },
	m_near{ _near },
	m_far{ _far },
	m_transform{}
{
	m_transform.SetPosition(pos);
	InitProjectionMat();
	ComputeViewMatrix();
}

Camera::~Camera()
{

}

void Camera::InitProjectionMat()
{
	float ratio = m_viewWidth / m_viewHeight;
	float t{ tanf((m_fov * (float)PI / 180.f) / 2.f) };
	if (m_type == EProjectType::PERSPECTIVE)
	{
		m_projectMat.m_matrix[0][0] = 1.f / (t * ratio);
		m_projectMat.m_matrix[0][1] = 0.f;
		m_projectMat.m_matrix[0][2] = 0.f;
		m_projectMat.m_matrix[0][3] = 0.f;

		m_projectMat.m_matrix[1][0] = 0.f;
		m_projectMat.m_matrix[1][1] = 1.f / t;
		m_projectMat.m_matrix[1][2] = 0.f;
		m_projectMat.m_matrix[1][3] = 0.f;

		m_projectMat.m_matrix[2][0] = 0.f;
		m_projectMat.m_matrix[2][1] = 0.f;
		m_projectMat.m_matrix[2][2] = (m_far) / (m_far - m_near);
		m_projectMat.m_matrix[2][3] = -(m_near * m_far) / (m_far - m_near);

		m_projectMat.m_matrix[3][0] = 0.f;
		m_projectMat.m_matrix[3][1] = 0.f;
		m_projectMat.m_matrix[3][2] = 1.f;
		m_projectMat.m_matrix[3][3] = 0.f;
	}
	else if (m_type == EProjectType::ORTHOGRAPHIC)
	{
		float left{ -m_viewWidth / 2 };
		float right{ m_viewWidth / 2 };
		float top{ -m_viewHeight / 2 };
		float bottom{ m_viewHeight / 2 };

		m_near = 0.1f;
		m_far = 1.f;

		m_projectMat.m_matrix[0][0] = 2.f / (right - left);
		m_projectMat.m_matrix[0][1] = 0.f;
		m_projectMat.m_matrix[0][2] = 0.f;
		m_projectMat.m_matrix[0][3] = 0.f;
		m_projectMat.m_matrix[1][0] = 0.f;
		m_projectMat.m_matrix[1][1] = 2.f / (top - bottom);
		m_projectMat.m_matrix[1][2] = 0.f;
		m_projectMat.m_matrix[1][3] = 0.f;
		m_projectMat.m_matrix[2][0] = 0.f;
		m_projectMat.m_matrix[2][1] = 0.f;
		m_projectMat.m_matrix[2][2] = 1.f / (m_far - m_near);
		m_projectMat.m_matrix[2][3] = 0.f;
		m_projectMat.m_matrix[3][0] = -1.f * ((right + left) / (right - left));
		m_projectMat.m_matrix[3][1] = -1.f * ((top + bottom) / (top - bottom));
		m_projectMat.m_matrix[3][2] = -1.f * ((m_far + m_near) / (m_far - m_near));
		m_projectMat.m_matrix[3][3] = 0.f;
	}
}

void Camera::SetType(EProjectType type)
{
	if (type == m_type)
		return;
	m_type = type;
	InitProjectionMat();
}

void Camera::SetFov(float fov)
{
	m_fov = fov;
	InitProjectionMat();
}

void Camera::SetViewSize(float width, float height, float _near, float _far)
{
	m_viewWidth = width;
	m_viewHeight = height;
	m_near = _near;
	m_far = _far;
	InitProjectionMat();
}

EProjectType Camera::GetType() const	{ return m_type; }
Mat4 Camera::GetViewMat() const			{ return m_viewMat; }
Mat4 Camera::GetProjectMat() const		{ return m_projectMat; }
Vec3 Camera::GetPosition() const		{ return m_transform.GetPosition(); }
Quaternion Camera::GetRotation() const	{ return m_transform.GetRotation(); }
Vec3 Camera::GetForward() const			{ return m_transform.GetForward(); }
Vec3 Camera::GetRight() const			{ return m_transform.GetRight(); }
Vec3 Camera::GetUp() const				{ return m_transform.GetUp(); }

void Camera::SetPosition(Vec3 pos)
{
	m_transform.SetPosition(pos);
	ComputeViewMatrix();
}

void Camera::SetRotation(Quaternion rot)
{
	m_transform.SetRotation(rot);
	ComputeViewMatrix();
}

void Camera::ComputeViewMatrix() {

	m_viewMat =
	{
		GetRight().m_x,		GetRight().m_y,		GetRight().m_z,		-GetRight().DotProduct(GetPosition()),
		GetUp().m_x,		GetUp().m_y,		GetUp().m_z,		-GetUp().DotProduct(GetPosition()),
		GetForward().m_x,	GetForward().m_y,	GetForward().m_z,	-GetForward().DotProduct(GetPosition()),
		0.f,				0.f,				0.f,				1.f
	};

//	m_viewMat = m_transform.GetWorldMat().GetInverse();
}

void Camera::LookAt(Vec3 pos)
{

	//Vec3 up{ 0.f,1.f,0.f };
	//Vec3 target{ m_transform.GetPosition() + m_target };


	//m_forward = m_target.GetNormalized(); // The "forward" vector.
	//m_right = up.CrossProduct(m_forward).GetNormalized();	// The "right" vector.
	//m_up = m_forward.CrossProduct(m_right); // The "up" vector.

	//m_viewMat =
	//{
	//	GetRight().m_x,		GetRight().m_y,		GetRight().m_z,		-GetRight().DotProduct(m_transform.GetPosition()),
	//	GetUp().m_x,		GetUp().m_y,		GetUp().m_z,		-GetUp().DotProduct(m_transform.GetPosition()),
	//	GetForward().m_x,	GetForward().m_y,	GetForward().m_z,	-GetForward().DotProduct(m_transform.GetPosition()),
	//	0.f,				0.f,				0.f,				1.f
	//};
	//Vec3 dir = (pos - m_transform.GetPosition()).GetNormalized();
	//SetRotation(Quaternion::Euler(dir));
	//ComputeViewMatrix();
}

void Camera::Translate(Vec3 position) {
	
	m_transform.Translate(position);
	ComputeViewMatrix();
}

void Camera::Rotate(Quaternion rot) {

	m_transform.Rotate(rot);
	ComputeViewMatrix();
}

void Camera::Check()
{
	m_transform;
	m_viewMat;
	m_projectMat;
	Vec2 pos;
	pos += Vec2{ 1.f, 2.f };
}