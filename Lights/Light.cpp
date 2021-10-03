#include "Light.h"

using namespace engine::core::maths;

Light::Light(Vec4 pos, Vec4 dir, Vec4 color, float constant, float linear, float quadratic, float cutOff) {
	m_transform.SetPosition({ pos.m_x, pos.m_y, pos.m_z });
	m_info.lightPos = pos;
	m_info.lightDir = dir;
	m_info.lightColor = color;
	m_info.constant = constant;
	m_info.linear = linear;
	m_info.quadratic = quadratic;
	m_info.cutOff = cutOff;
}

Light::Light(const Light& lLight) :
	m_transform{ lLight.m_transform },
	m_info { lLight.m_info }
{}

Light::Light(Light&& rLight) noexcept :
	m_transform{ rLight.m_transform },
	m_info{ rLight.m_info }
{}

Light::~Light()
{}

void Light::Translate(engine::core::maths::Vec3 pos) {

	m_transform.Translate(pos);
	m_info.lightPos = { m_transform.GetPosition().m_x, m_transform.GetPosition().m_y, m_transform.GetPosition().m_z, m_info.lightPos.m_w};
}

void Light::Rotate(engine::core::maths::Quaternion rot) {

	m_transform.Rotate(rot);
	m_info.lightDir = { 2 * (m_transform.GetRotation().m_x * m_transform.GetRotation().m_z - m_transform.GetRotation().m_w * m_transform.GetRotation().m_y),
						2 * (m_transform.GetRotation().m_y * m_transform.GetRotation().m_z + m_transform.GetRotation().m_w * m_transform.GetRotation().m_x),
						1 - 2 * (m_transform.GetRotation().m_x * m_transform.GetRotation().m_x + m_transform.GetRotation().m_y * m_transform.GetRotation().m_y),
						m_info.lightDir.m_w};
}

const Transform Light::GetTransform() const { return m_transform; }
Mat4 Light::GetWorldMat() const				{ return m_transform.GetWorldMat(); }
ConstantbufferLight Light::GetInfo() const  { return m_info; }

void Light::SetPositionAndRotation(engine::core::maths::Vec3 pos, engine::core::maths::Quaternion rot) {

	m_transform.SetTRS(pos, rot, {0.0f, 0.0f, 0.0f});
	m_info.lightPos = { m_transform.GetPosition().m_x, m_transform.GetPosition().m_y, m_transform.GetPosition().m_z, m_info.lightPos.m_w };
	m_info.lightDir = { 2 * (m_transform.GetRotation().m_x * m_transform.GetRotation().m_z - m_transform.GetRotation().m_w * m_transform.GetRotation().m_y),
						2 * (m_transform.GetRotation().m_y * m_transform.GetRotation().m_z + m_transform.GetRotation().m_w * m_transform.GetRotation().m_x),
						1 - 2 * (m_transform.GetRotation().m_x * m_transform.GetRotation().m_x + m_transform.GetRotation().m_y * m_transform.GetRotation().m_y),
						m_info.lightDir.m_w};
}

void Light::SetColor(engine::core::maths::Vec4 col) {

	m_info.lightColor = col;
}

void Light::SetConstant(float constant) {

	m_info.constant = constant;
}

void Light::SetLinear(float linear) {

	m_info.linear = linear;
}

void Light::SetQuadratic(float quadratic) {

	m_info.quadratic = quadratic;
}

void Light::SetAttenuation(float constant, float linear, float quadratic) {

	m_info.constant = constant;
	m_info.linear = linear;
	m_info.quadratic = quadratic;
}

void Light::SetCutOff(float cutOff) {

	m_info.cutOff = cutOff;
}


Light& Light::operator=(const Light& other) {

	m_transform = other.m_transform;
	m_info = other.m_info;

	return *this;
}

Light& Light::operator=(Light&& other) noexcept {

	m_transform = other.m_transform;
	m_info = other.m_info;

	return *this;
}

bool Light::operator==(const Light& other) const {

	return (m_transform == other.m_transform) && (m_info == other.m_info);
}
bool Light::operator!=(const Light& other) const {

	return (m_transform != other.m_transform) || (m_info != other.m_info);
}