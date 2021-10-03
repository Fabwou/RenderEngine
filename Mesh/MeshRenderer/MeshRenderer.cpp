#include "MeshRenderer.h"

using namespace Microsoft::WRL;

MeshRenderer::MeshRenderer(Device* device, Model* model, PSO* pso, std::vector<ConstantBuffer*> cb, std::vector<Texture*> textures) :
	m_model{ model },
	m_pso{ pso },
	m_constantBuffers{ cb },
	m_textures{ textures },
	m_mainDescriptorHeap{ nullptr },
	m_heapDesc{},
	m_srvHandleSize{}
{
	if (m_textures.size() > 0)
	{
		HRESULT hr;

		m_heapDesc.NumDescriptors = m_textures.size();
		m_heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		m_heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		hr = device->GetDevice()->CreateDescriptorHeap(&m_heapDesc, IID_PPV_ARGS(&m_mainDescriptorHeap));
		if (FAILED(hr))
		{
			MessageBox(0, L"Texture: failed to create descriptor heap",
				L"Error", MB_OK);
		}
		m_srvHandleSize = device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		for (int i{ 0 }; i < m_textures.size(); ++i)
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_mainDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), i, m_srvHandleSize);
			device->GetDevice()->CreateShaderResourceView(m_textures[i]->GetTextureBuffer().Get(), m_textures[i]->GetSrvDesc(), srvHandle);
		}
	}
}

MeshRenderer::MeshRenderer(const MeshRenderer& other):
	m_model{ other.m_model },
	m_pso{ other.m_pso },
	m_constantBuffers{ other.m_constantBuffers },
	m_textures{ other.m_textures },
	m_mainDescriptorHeap{ other.m_mainDescriptorHeap },
	m_heapDesc{ other.m_heapDesc },
	m_srvHandleSize{ other.m_srvHandleSize }
{}

MeshRenderer::MeshRenderer(MeshRenderer&& other) noexcept:
	m_model{ other.m_model },
	m_pso{ other.m_pso },
	m_constantBuffers{ other.m_constantBuffers },
	m_textures{ other.m_textures },
	m_mainDescriptorHeap{ other.m_mainDescriptorHeap },
	m_heapDesc{ other.m_heapDesc },
	m_srvHandleSize{ other.m_srvHandleSize }
{}

MeshRenderer::~MeshRenderer()
{}

Model* MeshRenderer::GetModel() const										{ return m_model; }
RootSignature* MeshRenderer::GetRootSignature() const						{ return m_pso->GetRootSignature(); }
PSO* MeshRenderer::GetPso() const											{ return m_pso; }
std::vector<Shader*> MeshRenderer::GetShaders() const						{ return m_pso->GetShaders(); }
std::vector<ConstantBuffer*> MeshRenderer::GetConstantBuffers() const		{ return m_constantBuffers; }
std::vector<Texture*> MeshRenderer::GetTextures() const						{ return m_textures; }
ComPtr<ID3D12DescriptorHeap> MeshRenderer::GetMainDescriptorHeap() const	{ return m_mainDescriptorHeap; }

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& other) {
	m_model = other.m_model;
	m_pso = other.m_pso;
	m_constantBuffers = other.m_constantBuffers;
	m_textures = other.m_textures;
	m_mainDescriptorHeap = other.m_mainDescriptorHeap;
	m_heapDesc = other.m_heapDesc;
	m_srvHandleSize = other.m_srvHandleSize;

	return *this;
}

MeshRenderer& MeshRenderer::operator=(MeshRenderer&& other) noexcept {
	m_model = other.m_model;
	m_pso = other.m_pso;
	m_constantBuffers = other.m_constantBuffers;
	m_textures = other.m_textures;
	m_mainDescriptorHeap = other.m_mainDescriptorHeap;
	m_heapDesc = other.m_heapDesc;
	m_srvHandleSize = other.m_srvHandleSize;

	return *this;
}

bool MeshRenderer::operator==(const MeshRenderer& other) const {

	return (m_model == other.m_model) && (m_pso == other.m_pso) && (m_textures == other.m_textures);

}

bool MeshRenderer::operator!=(const MeshRenderer& other) const {

	return (m_model != other.m_model) || (m_pso != other.m_pso) || (m_textures != other.m_textures);

}