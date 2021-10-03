#include "FBXLoader.h"
#include "../Core/Maths/Vectors/Vec4/Vec4.h"
#include "BasicObjectLoader.h"

using namespace engine::core::maths;

FBXLoader::FBXLoader(Device* device) :
	m_device{ device },
	m_scene{ nullptr }
{}

FBXLoader::~FBXLoader()
{
	m_returnedModels.clear();
	if (m_scene) {
		m_scene->destroy();
		m_scene = nullptr;
	}
}

std::vector<Model*> FBXLoader::ExtractFile(LPCWSTR path)
{
	std::vector<Model*> models;

	const size_t size = wcslen(path) + 1;
	size_t returnValue = 0;
	std::vector<char> charBuffer(size);
	size_t vectorSizeInBytes = sizeof(char) * charBuffer.size();
	wcstombs_s(&returnValue, &charBuffer[0], vectorSizeInBytes, path, size);

	FILE* fp;
	errno_t err;

	if ((err = fopen_s(&fp, &charBuffer[0], "rb")) != 0) {
		MessageBox(0, L"Failed to open FBX file",
			L"Error", MB_OK);

		return models;
	}

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto* content = new ofbx::u8[file_size];
	fread(content, 1, file_size, fp);
	m_scene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
	if (!m_scene) {
		MessageBox(0, L"Couldn't load fbx",
			L"Error", MB_OK);
		OutputDebugStringA(ofbx::getError());
	}
	else {
		std::vector<char> temp(charBuffer);
		temp[temp.size() - 4] = 'o';
		temp[temp.size() - 3] = 'b';
		temp[temp.size() - 2] = 'j';
//		saveAsOBJ(*m_scene, &temp[0]);
	}
	delete[] content;
	fclose(fp);

	std::vector<Vertex> vertices;
	std::vector<int> indices;
	Vec3 pos;
	Vec3 norm;
	Vec2 UV;
	Vec3 tan;
	bool hasNormal, hasUV, hasTangent = false;

	for (unsigned int i = 0; i < m_scene->getMeshCount(); ++i) {
		for (unsigned int j = 0; j < m_scene->getMesh(i)->getGeometry()->getVertexCount(); ++j) {
			pos.m_x = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getVertices()[j].x);
			pos.m_y = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getVertices()[j].y);
			pos.m_z = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getVertices()[j].z);

			if (m_scene->getMesh(i)->getGeometry()->getNormals() != nullptr) {
				norm.m_x = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getNormals()[j].x);
				norm.m_y = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getNormals()[j].y);
				norm.m_z = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getNormals()[j].z);
				hasNormal = true;
			}

			if (m_scene->getMesh(i)->getGeometry()->getUVs() != nullptr) {
				UV.m_x = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getUVs()[j].x);
				UV.m_y = 1.0f - static_cast<float>(m_scene->getMesh(i)->getGeometry()->getUVs()[j].y);
				hasUV = true;
			}

			if (m_scene->getMesh(i)->getGeometry()->getTangents() != nullptr) {
				tan.m_x = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getTangents()[j].x);
				tan.m_y = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getTangents()[j].y);
				tan.m_z = static_cast<float>(m_scene->getMesh(i)->getGeometry()->getTangents()[j].z);
				hasTangent = true;
			}
			vertices.push_back(Vertex(pos, norm, UV, tan));
			indices.push_back(j);

		}

		if (!hasTangent)
			ComputeTangentsAndBitangents(vertices);
		else
			ComputeBitangents(vertices);

		models.push_back(new Model(m_device, m_scene->getMesh(i)->name, vertices, indices));

		vertices.clear();
		indices.clear();
	}
	if (m_scene) {
		m_scene->destroy();
		m_scene = nullptr;
	}
	return models;
}

bool FBXLoader::saveAsOBJ(ofbx::IScene& scene, const char* path)
{
	FILE* fp;
	errno_t err;

	if ((err = fopen_s(&fp, path, "wb")) != 0)
		return false;

	int obj_idx = 0;
	int indices_offset = 0;
	int normals_offset = 0;
	int mesh_count = scene.getMeshCount();
	for (int i = 0; i < mesh_count; ++i)
	{
		fprintf(fp, "o obj%d\ng grp%d\n", i, obj_idx);

		const ofbx::Mesh& mesh = *scene.getMesh(i);
		const ofbx::Geometry& geom = *mesh.getGeometry();
		int vertex_count = geom.getVertexCount();
		const ofbx::Vec3* vertices = geom.getVertices();
		for (int i = 0; i < vertex_count; ++i)
		{
			ofbx::Vec3 v = vertices[i];
			fprintf(fp, "v %f %f %f\n", v.x, v.y, v.z);
		}

		bool has_normals = geom.getNormals() != nullptr;
		if (has_normals)
		{
			const ofbx::Vec3* normals = geom.getNormals();
			int count = geom.getIndexCount();

			for (int i = 0; i < count; ++i)
			{
				ofbx::Vec3 n = normals[i];
				fprintf(fp, "vn %f %f %f\n", n.x, n.y, n.z);
			}
		}

		bool has_uvs = geom.getUVs() != nullptr;
		if (has_uvs)
		{
			const ofbx::Vec2* uvs = geom.getUVs();
			int count = geom.getIndexCount();

			for (int i = 0; i < count; ++i)
			{
				ofbx::Vec2 uv = uvs[i];
				fprintf(fp, "vt %f %f\n", uv.x, uv.y);
			}
		}

		const int* faceIndices = geom.getFaceIndices();
		int index_count = geom.getIndexCount();
		bool new_face = true;
		for (int i = 0; i < index_count; ++i)
		{
			if (new_face)
			{
				fputs("f ", fp);
				new_face = false;
			}
			int idx = (faceIndices[i] < 0) ? -faceIndices[i] : (faceIndices[i] + 1);
			int vertex_idx = indices_offset + idx;
			fprintf(fp, "%d", vertex_idx);

			if (has_uvs)
			{
				int uv_idx = normals_offset + i + 1;
				fprintf(fp, "/%d", uv_idx);
			}
			else
			{
				fprintf(fp, "/");
			}

			if (has_normals)
			{
				int normal_idx = normals_offset + i + 1;
				fprintf(fp, "/%d", normal_idx);
			}
			else
			{
				fprintf(fp, "/");
			}

			new_face = faceIndices[i] < 0;
			fputc(new_face ? '\n' : ' ', fp);
		}

		indices_offset += vertex_count;
		normals_offset += index_count;
		++obj_idx;
	}
	fclose(fp);
	return true;
}