#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../Mesh/Transform/Transform.h"
#include "../ConstantBuffers.h"
#include "../D3D12/ConstantBuffer/ConstantBuffer.h"

class Light {

public:

	Light(engine::core::maths::Vec4 pos = Vec4::Zero(), engine::core::maths::Vec4 dir = Vec4::Zero(), engine::core::maths::Vec4 color = Vec4::Zero(),
			float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f, float cutOff = 12.5f);
	Light(const Light& lLight);
	Light(Light&& rLight) noexcept;
	~Light();

	void Translate(engine::core::maths::Vec3 pos);
	void Rotate(engine::core::maths::Quaternion rot);

	const Transform GetTransform() const;
	engine::core::maths::Mat4 GetWorldMat() const;
	ConstantbufferLight GetInfo() const;

	void SetPositionAndRotation(engine::core::maths::Vec3 pos, engine::core::maths::Quaternion rot);
	void SetColor(engine::core::maths::Vec4 col);
	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	void SetAttenuation(float constant, float linear, float quadratic);
	void SetCutOff(float cutOff);

	Light& operator=(const Light& other);
	Light& operator=(Light&& other) noexcept;
	bool operator==(const Light& other) const;
	bool operator!=(const Light& other) const;

private:

	Transform m_transform;
	ConstantbufferLight m_info;

};

#endif