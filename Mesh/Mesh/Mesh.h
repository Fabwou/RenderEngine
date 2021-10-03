#ifndef _MESH_H_
#define _MESH_H_

#include "../MeshRenderer/MeshRenderer.h"
#include "../Transform/Transform.h"
#include "../../Lights/Light.h"

#include <vector>
#include <string>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Mesh
{
	public:
		Mesh(MeshRenderer* renderer);
		Mesh(MeshRenderer* renderer, Transform transform);
		Mesh(MeshRenderer* renderer, Transform transform, ConstantbufferMultipleLight nearestLights);
		Mesh(const Mesh& other);
		Mesh(Mesh&& other) noexcept;
		~Mesh();

		Transform GetTransform() const;
		Transform& GetTransform();
		MeshRenderer* GetMeshRenderer() const;
		ConstantbufferMultipleLight GetNearestLights() const;

		void SetNearestLights(ConstantbufferMultipleLight nearestLights);
		ConstantbufferMultipleLight FindNearestLights(std::vector<Light*> allLights);

		Mesh& operator=(const Mesh& other);
		Mesh& operator=(Mesh&& other) noexcept;
		bool operator==(const Mesh& other) const;
		bool operator!=(const Mesh& other) const;

	private:
		Transform m_transform;
		MeshRenderer* m_meshRenderer;
		ConstantbufferMultipleLight m_nearestLights;
};
#endif