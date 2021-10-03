#include "Mesh.h"

Mesh::Mesh(MeshRenderer* renderer):
	m_transform{},
	m_meshRenderer{ renderer },
	m_nearestLights{}
{}

Mesh::Mesh(MeshRenderer* renderer, Transform transform):
	m_transform{ transform },
	m_meshRenderer{ renderer },
	m_nearestLights{}
{}

Mesh::Mesh(MeshRenderer* renderer, Transform transform, ConstantbufferMultipleLight nearestLights):
	m_transform{ transform },
	m_meshRenderer{ renderer },
	m_nearestLights{ nearestLights }
{}

Mesh::Mesh(const Mesh& other):
	m_transform{ other.m_transform },
	m_meshRenderer{ other.m_meshRenderer },
	m_nearestLights{ other.m_nearestLights }
{}

Mesh::Mesh(Mesh&& other) noexcept :
	m_transform{ other.m_transform },
	m_meshRenderer{ other.m_meshRenderer },
	m_nearestLights{ other.m_nearestLights }
{}

Mesh::~Mesh()
{}

Transform Mesh::GetTransform() const						{ return m_transform; }
Transform& Mesh::GetTransform()								{ return m_transform; }
MeshRenderer* Mesh::GetMeshRenderer() const					{ return m_meshRenderer; }
ConstantbufferMultipleLight Mesh::GetNearestLights() const	{ return m_nearestLights; }

void Mesh::SetNearestLights(ConstantbufferMultipleLight nearestLights)
{
	m_nearestLights = nearestLights;
}

ConstantbufferMultipleLight Mesh::FindNearestLights(std::vector<Light*> allLights)
{
	for (unsigned int i = 0; i < 8; ++i)
		m_nearestLights.lightInfo[i] = ConstantbufferLight();

	float test[8]{ -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f };
	for (unsigned int i = 0; i < allLights.size(); ++i) {

		float length = abs(allLights[i]->GetInfo().lightPos.GetMagnitudeSQ() - m_transform.GetPosition().GetMagnitudeSQ());
		for (unsigned int j = 0; j < 8; ++j) {

			if (test[j] < 0.0f) {
				test[j] = length;
				m_nearestLights.lightInfo[j] = allLights[i]->GetInfo();
				j += 99;
			}
			else if (length < test[j]) {
				for (unsigned int k = 7; k > j; --k) {
					test[k] = test[k - 1];
					m_nearestLights.lightInfo[k] = m_nearestLights.lightInfo[k - 1];
				}
				test[j] = length;
				m_nearestLights.lightInfo[j] = allLights[i]->GetInfo();
				j += 99;
			}
		}
	}
	return m_nearestLights;
}

Mesh& Mesh::operator=(const Mesh& other) {

	m_transform = other.m_transform;
	m_meshRenderer = other.m_meshRenderer;
	m_nearestLights = other.m_nearestLights;

	return *this;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {

	m_transform = other.m_transform;
	m_meshRenderer = other.m_meshRenderer;
	m_nearestLights = other.m_nearestLights;

	return *this;
}

bool Mesh::operator==(const Mesh& other) const {

	return (m_transform == other.m_transform) && (m_meshRenderer == other.m_meshRenderer) && (m_nearestLights == other.m_nearestLights);
}

bool Mesh::operator!=(const Mesh& other) const {

	return (m_transform != other.m_transform) || (m_meshRenderer != other.m_meshRenderer) || (m_nearestLights != other.m_nearestLights);
}