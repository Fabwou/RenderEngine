#include "BasicObjectLoader.h"

Model* LoadCube(Device* device) {

	std::vector<Vertex> vList
	{
		// front face
		Vertex{ { -0.5f,  0.5f, -0.5f },   {  0.0f,  0.0f,  -1.0f },   { 0.0f, 0.0f } },
		Vertex{ {  0.5f, -0.5f, -0.5f },   {  0.0f,  0.0f,  -1.0f },   { 1.0f, 1.0f } },
		Vertex{ { -0.5f, -0.5f, -0.5f },   {  0.0f,  0.0f,  -1.0f },   { 0.0f, 1.0f } },
		Vertex{ {  0.5f,  0.5f, -0.5f },   {  0.0f,  0.0f,  -1.0f },   { 1.0f, 0.0f } },

		// left side face
		Vertex{ {  0.5f, -0.5f, -0.5f },   {  1.0f,  0.0f,  0.0f },   { 0.0f, 1.0f } },
		Vertex{ {  0.5f,  0.5f,  0.5f },   {  1.0f,  0.0f,  0.0f },   { 1.0f, 0.0f } },
		Vertex{ {  0.5f, -0.5f,  0.5f },   {  1.0f,  0.0f,  0.0f },   { 1.0f, 1.0f } },
		Vertex{ {  0.5f,  0.5f, -0.5f },   {  1.0f,  0.0f,  0.0f },   { 0.0f, 0.0f } },

		// right side face
		Vertex{ { -0.5f,  0.5f,  0.5f },   { -1.0f,  0.0f,  0.0f },   { 0.0f, 0.0f } },
		Vertex{ { -0.5f, -0.5f, -0.5f },   { -1.0f,  0.0f,  0.0f },   { 1.0f, 1.0f } },
		Vertex{ { -0.5f, -0.5f,  0.5f },   { -1.0f,  0.0f,  0.0f },   { 0.0f, 1.0f } },
		Vertex{ { -0.5f,  0.5f, -0.5f },   { -1.0f,  0.0f,  0.0f },   { 1.0f, 0.0f } },

		// back face
		Vertex{ {  0.5f,  0.5f,  0.5f },   {  0.0f,  0.0f, 1.0f },   { 0.0f, 0.0f } },
		Vertex{ { -0.5f, -0.5f,  0.5f },   {  0.0f,  0.0f, 1.0f },   { 1.0f, 1.0f } },
		Vertex{ {  0.5f, -0.5f,  0.5f },   {  0.0f,  0.0f, 1.0f },   { 0.0f, 1.0f } },
		Vertex{ { -0.5f,  0.5f,  0.5f },   {  0.0f,  0.0f, 1.0f },   { 1.0f, 0.0f } },

		// top face
		Vertex{ { -0.5f,  0.5f, -0.5f },   {  0.0f,  1.0f,  0.0f },   { 0.0f, 1.0f } },
		Vertex{ {  0.5f,  0.5f,  0.5f },   {  0.0f,  1.0f,  0.0f },   { 1.0f, 0.0f } },
		Vertex{ {  0.5f,  0.5f, -0.5f },   {  0.0f,  1.0f,  0.0f },   { 1.0f, 1.0f } },
		Vertex{ { -0.5f,  0.5f,  0.5f },   {  0.0f,  1.0f,  0.0f },   { 0.0f, 0.0f } },

		// bottom face
		Vertex{ {  0.5f, -0.5f,  0.5f },   {  0.0f, -1.0f,  0.0f },   { 0.0f, 0.0f } },
		Vertex{ { -0.5f, -0.5f, -0.5f },   {  0.0f, -1.0f,  0.0f },   { 1.0f, 1.0f } },
		Vertex{ {  0.5f, -0.5f, -0.5f },   {  0.0f, -1.0f,  0.0f },   { 0.0f, 1.0f } },
		Vertex{ { -0.5f, -0.5f,  0.5f },   {  0.0f, -1.0f,  0.0f },   { 1.0f, 0.0f } },
	};

	ComputeTangentsAndBitangents(vList);

	std::vector<int> iList
	{
		// front face
		0, 1, 2, // first triangle
		0, 3, 1, // second triangle
		// left face
		4, 5, 6, // first triangle
		4, 7, 5, // second triangle
		// right face
		8, 9, 10, // first triangle
		8, 11, 9, // second triangle
		//back face
		12, 13, 14, // first triangle
		12, 15, 13, // second triangle
		// top face
		16, 17, 18, // first triangle
		16, 19, 17, // second triangle
		// bottom face
		20, 21, 22, // first triangle
		20, 23, 21, // second triangle
	};

	return new Model(device, L"Cube", vList, iList);
}

void ComputeTangents(std::vector<Vertex>& vertices) {

	for (int i = 0; i < vertices.size(); i += 3) {

		// Shortcuts for vertices
		Vec3& v0 = vertices[i + 0].m_position;
		Vec3& v1 = vertices[i + 1].m_position;
		Vec3& v2 = vertices[i + 2].m_position;

		// Shortcuts for UVs
		Vec2& uv0 = vertices[i + 0].m_texCoord;
		Vec2& uv1 = vertices[i + 1].m_texCoord;
		Vec2& uv2 = vertices[i + 2].m_texCoord;

		// Edges of the triangle : postion delta
		Vec3 deltaPos1 = v1 - v0;
		Vec3 deltaPos2 = v2 - v0;

		// UV delta
		Vec2 deltaUV1 = uv1 - uv0;
		Vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
		Vec3 tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y) * r;

		// Set the same tangent for all three vertices of the triangle.
		vertices[i + 0].m_tangent = tangent;
		vertices[i + 1].m_tangent = tangent;
		vertices[i + 2].m_tangent = tangent;
	}
}

void ComputeBitangents(std::vector<Vertex>& vertices) {

	for (int i = 0; i < vertices.size(); i += 3) {

		// Shortcuts for vertices
		Vec3& v0 = vertices[i + 0].m_position;
		Vec3& v1 = vertices[i + 1].m_position;
		Vec3& v2 = vertices[i + 2].m_position;

		// Shortcuts for UVs
		Vec2& uv0 = vertices[i + 0].m_texCoord;
		Vec2& uv1 = vertices[i + 1].m_texCoord;
		Vec2& uv2 = vertices[i + 2].m_texCoord;

		// Edges of the triangle : postion delta
		Vec3 deltaPos1 = v1 - v0;
		Vec3 deltaPos2 = v2 - v0;

		// UV delta
		Vec2 deltaUV1 = uv1 - uv0;
		Vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
		Vec3 bitangent = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x) * r;

		// Same thing for binormals
		vertices[i + 0].m_bitangent = bitangent;
		vertices[i + 1].m_bitangent = bitangent;
		vertices[i + 2].m_bitangent = bitangent;

	}
}

void ComputeTangentsAndBitangents(std::vector<Vertex>& vertices) {

	for (int i = 0; i < vertices.size(); i += 3) {

		// Shortcuts for vertices
		Vec3& v0 = vertices[i + 0].m_position;
		Vec3& v1 = vertices[i + 1].m_position;
		Vec3& v2 = vertices[i + 2].m_position;

		// Shortcuts for UVs
		Vec2& uv0 = vertices[i + 0].m_texCoord;
		Vec2& uv1 = vertices[i + 1].m_texCoord;
		Vec2& uv2 = vertices[i + 2].m_texCoord;

		// Edges of the triangle : postion delta
		Vec3 deltaPos1 = v1 - v0;
		Vec3 deltaPos2 = v2 - v0;

		// UV delta
		Vec2 deltaUV1 = uv1 - uv0;
		Vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
		Vec3 tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y) * r;
		Vec3 bitangent = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x) * r;

		// Set the same tangent for all three vertices of the triangle.
		vertices[i + 0].m_tangent = tangent;
		vertices[i + 1].m_tangent = tangent;
		vertices[i + 2].m_tangent = tangent;

		// Same thing for binormals
		vertices[i + 0].m_bitangent = bitangent;
		vertices[i + 1].m_bitangent = bitangent;
		vertices[i + 2].m_bitangent = bitangent;

	}
}