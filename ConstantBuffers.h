#ifndef _CONSTANT_BUFFERS_H_
#define _CONSTANT_BUFFERS_H_

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "Core/Maths/Matrices/Mat4.h"
#include "Core/Maths/Vectors/Vec2/Vec2.h"

struct ConstantBufferPerObject {
	engine::core::maths::Mat4 wMat;
	engine::core::maths::Mat4 vMat;
	engine::core::maths::Mat4 pMat;
	engine::core::maths::Mat4 mvp;
	engine::core::maths::Mat4 invWMat;
	engine::core::maths::Vec3 cameraPos;
	float pad;
};

struct ConstantBufferMaterial {
	engine::core::maths::Vec4 ambientColor;
	engine::core::maths::Vec4 emissiveColor;
	float glossiness;
	engine::core::maths::Vec3 pad;
};

struct ConstantBufferGeometryShader {

	float time = 0.0f;
	float magnitude = 1.0f;
	engine::core::maths::Vec2 pad;
};

struct ConstantbufferLight {

	engine::core::maths::Vec4 lightColor;
	engine::core::maths::Vec4 lightPos;
	engine::core::maths::Vec4 lightDir;
	float constant;
	float linear;
	float quadratic;
	float cutOff;

	ConstantbufferLight():
		lightColor{0.0f, 0.0f, 0.0f, 0.0f},
		lightPos{0.0f, 0.0f, 0.0f, 0.0f},
		lightDir{ 0.0f, 0.0f, 0.0f, 0.0f },
		constant{1.0f},
		linear{0.09f},
		quadratic{0.032},
		cutOff{12.5f}
	{}

	ConstantbufferLight(const ConstantbufferLight& other):
		lightColor{other.lightColor},
		lightPos{other.lightPos},
		lightDir{ other.lightDir },
		constant{ other.constant },
		linear{ other.linear },
		quadratic{ other.quadratic },
		cutOff{ other.cutOff }
	{}

	ConstantbufferLight(ConstantbufferLight&& other) noexcept :
		lightColor{ other.lightColor },
		lightPos{ other.lightPos },
		lightDir{ other.lightDir },
		constant{ other.constant },
		linear{ other.linear },
		quadratic{ other.quadratic },
		cutOff{ other.cutOff }
	{}

	ConstantbufferLight& operator=(const ConstantbufferLight& other) {

		lightColor = other.lightColor;
		lightPos = other.lightPos;
		lightDir = other.lightDir;
		constant = other.constant;
		linear = other.linear;
		quadratic = other.quadratic;
		cutOff = other.cutOff;

		return *this;
	}
	ConstantbufferLight& operator=(ConstantbufferLight&& other) noexcept {

		lightColor = other.lightColor;
		lightPos = other.lightPos;
		lightDir = other.lightDir;
		constant = other.constant;
		linear = other.linear;
		quadratic = other.quadratic;
		cutOff = other.cutOff;

		return *this;
	}
	bool operator==(const ConstantbufferLight& other) const {

		return (lightColor == other.lightColor) && (lightPos == other.lightPos) && (lightDir == other.lightDir) &&
				(constant == other.constant) && (linear == other.linear) && (quadratic == other.quadratic) && (cutOff == other.cutOff);
	}
	bool operator!=(const ConstantbufferLight& other) const {

		return (lightColor != other.lightColor) || (lightPos != other.lightPos) || (lightDir != other.lightDir) ||
				(constant != other.constant) || (linear != other.linear) || (quadratic != other.quadratic) || (cutOff != other.cutOff);
	}
};

struct ConstantbufferMultipleLight {

	ConstantbufferLight lightInfo[8];

	ConstantbufferMultipleLight() {

		for (unsigned int i = 0; i < 8; ++i)
			lightInfo[i] = ConstantbufferLight();
	}

	ConstantbufferMultipleLight(const ConstantbufferMultipleLight& other) {

		for (unsigned int i = 0; i < 8; ++i)
			lightInfo[i] = other.lightInfo[i];
	}
	ConstantbufferMultipleLight(ConstantbufferMultipleLight&& other) noexcept {

		for (unsigned int i = 0; i < 8; ++i)
			lightInfo[i] = other.lightInfo[i];
	}
	ConstantbufferMultipleLight& operator=(const ConstantbufferMultipleLight& other) {

		for (unsigned int i = 0; i < 8; ++i)
			lightInfo[i] = other.lightInfo[i];

		return *this;
	}
	ConstantbufferMultipleLight& operator=(ConstantbufferMultipleLight&& other) noexcept {

		for (unsigned int i = 0; i < 8; ++i)
			lightInfo[i] = other.lightInfo[i];

		return *this;
	}
	bool operator==(const ConstantbufferMultipleLight& other) const {
		for (unsigned int i = 0; i < 8; ++i)
			if (lightInfo != other.lightInfo)
				return false;

		return true;
	}
	bool operator!=(const ConstantbufferMultipleLight& other) const {

		for (unsigned int i = 0; i < 8; ++i)
			if (lightInfo != other.lightInfo)
				return true;

		return false;
	}
};

#endif
