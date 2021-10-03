#ifndef _MESH_RENDERER_H_
#define _MESH_RENDERER_H_

#include "../Model/Model.h"
#include "../../D3D12/ConstantBuffer/ConstantBuffer.h"
#include "../../D3D12/Texture/Texture.h"
#include "../../D3D12/Pso/Pso.h"
#include <vector>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class MeshRenderer {

	public:

		MeshRenderer(Device* device, Model* model, PSO* pso, std::vector<ConstantBuffer*> cb, std::vector<Texture*> textures);
		MeshRenderer(const MeshRenderer& other);
		MeshRenderer(MeshRenderer&& other) noexcept;
		~MeshRenderer();

		Model* GetModel() const;
		RootSignature* GetRootSignature() const;
		PSO* GetPso() const;
		std::vector<Shader*> GetShaders() const;
		std::vector<ConstantBuffer*> GetConstantBuffers() const;
		std::vector<Texture*> GetTextures() const;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetMainDescriptorHeap() const;

		MeshRenderer& operator=(const MeshRenderer& other);
		MeshRenderer& operator=(MeshRenderer&& other) noexcept;
		bool operator==(const MeshRenderer& other) const;
		bool operator!=(const MeshRenderer& other) const;

	private:

		Model* m_model;
		PSO* m_pso;
		std::vector<ConstantBuffer*> m_constantBuffers;
		std::vector<Texture*> m_textures;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_mainDescriptorHeap;
		D3D12_DESCRIPTOR_HEAP_DESC m_heapDesc;
		UINT m_srvHandleSize;
};



#endif