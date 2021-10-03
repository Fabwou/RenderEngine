#ifndef _VERTEX_
#define _VERTEX_

#include "../Core/Maths/Vectors/Vec3/Vec3.h"
#include "../Core/Maths/Vectors/Vec2/Vec2.h"
using namespace engine::core::maths;

struct Vertex {

	Vec3 m_position;
	Vec3 m_normal;
	Vec2 m_texCoord;
	Vec3 m_tangent;
	Vec3 m_bitangent;

	Vertex() : m_position{ Vec3::Zero() }, m_normal{ Vec3::Zero() }, m_texCoord{ Vec2::Zero() }, m_tangent{ Vec3::Zero() }, m_bitangent{ Vec3::Zero() } {}
	Vertex(const Vertex& ver) : m_position{ ver.m_position }, m_normal{ ver.m_normal }, m_texCoord{ ver.m_texCoord }, m_tangent{ ver.m_tangent }, m_bitangent{ ver.m_bitangent } {}
	Vertex(Vertex&& ver) noexcept : m_position { ver.m_position }, m_normal{ ver.m_normal }, m_texCoord{ ver.m_texCoord }, m_tangent{ ver.m_tangent }, m_bitangent{ ver.m_bitangent } {}
	Vertex(Vec3 pos) : m_position{ pos }, m_normal{ Vec3::Zero() }, m_texCoord{ Vec2::Zero() }, m_tangent{ Vec3::Zero() }, m_bitangent{ Vec3::Zero() } {}
	Vertex(Vec3 pos, Vec2 texCoord) : m_position{ pos }, m_normal{ Vec3::Zero() }, m_texCoord{ texCoord }, m_tangent{ Vec3::Zero() }, m_bitangent{ Vec3::Zero() } {}
	Vertex(Vec3 pos, Vec3 nor) : m_position{ pos }, m_normal{ nor }, m_texCoord{ Vec2::Zero() }, m_tangent{ Vec3::Zero() }, m_bitangent{ Vec3::Zero() } {}
	Vertex(Vec3 pos, Vec3 nor, Vec2 texCoord) : m_position{ pos }, m_normal{ nor }, m_texCoord{ texCoord }, m_tangent{ Vec3::Zero() }, m_bitangent{ Vec3::Zero() } {}
	Vertex(Vec3 pos, Vec3 nor, Vec2 texCoord, Vec3 tan) : m_position{ pos }, m_normal{ nor }, m_texCoord{ texCoord }, m_tangent{ tan }, m_bitangent{ Vec3::Zero() } {}
	Vertex(Vec3 pos, Vec3 nor, Vec2 texCoord, Vec3 tan, Vec3 bin) : m_position{ pos }, m_normal{ nor }, m_texCoord{ texCoord }, m_tangent{ tan }, m_bitangent{ bin } {}

	Vertex& operator=(const Vertex& ver) {

		m_position = ver.m_position;
		m_normal = ver.m_normal;
		m_texCoord = ver.m_texCoord;
		m_tangent = ver.m_tangent;
		m_bitangent = ver.m_bitangent;
		return *this;
	}
	Vertex& operator=(Vertex&& ver) noexcept {

		m_position = ver.m_position;
		m_normal = ver.m_normal;
		m_texCoord = ver.m_texCoord;
		m_tangent = ver.m_tangent;
		m_bitangent = ver.m_bitangent;
		return *this;
	}
	bool operator==(const Vertex& ver) const {

		if (m_position == ver.m_position &&
			m_normal == ver.m_normal &&
			m_texCoord == ver.m_texCoord &&
			m_tangent == ver.m_tangent &&
			m_bitangent == ver.m_bitangent)
			return true;
		return false;
	}
	bool operator!=(const Vertex& ver) const {

		if (m_position == ver.m_position &&
			m_normal == ver.m_normal &&
			m_texCoord == ver.m_texCoord &&
			m_tangent == ver.m_tangent &&
			m_bitangent == ver.m_bitangent)
			return false;
		return true;
	}
};

#endif
