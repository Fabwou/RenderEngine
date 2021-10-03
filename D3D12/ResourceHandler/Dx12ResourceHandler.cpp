#include "Dx12ResourceHandler.h"

Dx12ResourceHandler::Dx12ResourceHandler(Device* device) : 
	m_device{ device },
	m_rootSignatures{},
	m_psos{},
	m_shaders{},
	m_textures{},
	m_meshRenderers{}
{}

Dx12ResourceHandler::~Dx12ResourceHandler()
{
	SAFE_DELETE_VECTOR(m_rootSignatures);
	SAFE_DELETE_VECTOR(m_psos);
	SAFE_DELETE_VECTOR(m_shaders);
	SAFE_DELETE_VECTOR(m_textures);
	SAFE_DELETE_VECTOR(m_meshRenderers);
}

RootSignature* Dx12ResourceHandler::GetRootSignature(std::vector<ConstantBuffer*>& cbs, unsigned int texNb, bool hasStaticSampler, D3D12_ROOT_SIGNATURE_FLAGS flags) {

	std::vector<CbComparator> comparator(cbs.size());
	for (unsigned int i = 0; i < cbs.size(); ++i)
		comparator[i] = { cbs[i]->GetShaderVisibility(), cbs[i]->GetRegisterNb() };
	for (unsigned int i = 0; i < m_rootSignatures.size(); ++i) {

		if ((m_rootSignatures[i]->GetComparators() == comparator) &&
			(m_rootSignatures[i]->GetTextureNb() == texNb) &&
			(m_rootSignatures[i]->HasStaticSampler() == hasStaticSampler) &&
			(m_rootSignatures[i]->GetFlags() == flags)) {
			return m_rootSignatures[i];
		}
	}

	m_rootSignatures.push_back(new RootSignature(m_device, cbs, texNb, hasStaticSampler, flags));
	return m_rootSignatures.back();
}

PSO* Dx12ResourceHandler::GetPso(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, RootSignature* rootSignature, std::vector<Shader*>& shaderTab)
{
	//for (unsigned int i = 0; i < m_psos.size(); ++i)
	//	if (*rootSignature == *m_psos[i]->GetRootSignature() && shaderTab == m_psos[i]->GetShaders())
	//		return m_psos[i];

	for (unsigned int i = 0; i < m_psos.size(); ++i)
		if (m_psos[i]->GetInputLayoutDesc()->NumElements == inputLayoutDesc->NumElements) {
			for (unsigned int j = 0; j < inputLayoutDesc->NumElements; ++j)
				if (m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].AlignedByteOffset == inputLayoutDesc->pInputElementDescs[j].AlignedByteOffset &&
					m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].Format == inputLayoutDesc->pInputElementDescs[j].Format &&
					m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].InputSlot == inputLayoutDesc->pInputElementDescs[j].InputSlot &&
					m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].InputSlotClass == inputLayoutDesc->pInputElementDescs[j].InputSlotClass &&
					m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].InstanceDataStepRate == inputLayoutDesc->pInputElementDescs[j].InstanceDataStepRate &&
					m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].SemanticIndex == inputLayoutDesc->pInputElementDescs[j].SemanticIndex &&
					m_psos[i]->GetInputLayoutDesc()->pInputElementDescs[j].SemanticName == inputLayoutDesc->pInputElementDescs[j].SemanticName &&
					*m_psos[i]->GetRootSignature() == *rootSignature && m_psos[i]->GetShaders() == shaderTab)
					return m_psos[i];
		}
	
	m_psos.push_back(new PSO(m_device, inputLayoutDesc, rootSignature, shaderTab));
	return m_psos.back();

}

Shader* Dx12ResourceHandler::GetShader(LPCWSTR path, D3D12_SHADER_VISIBILITY visibility) {

	for (unsigned int i = 0; i < m_shaders.size(); ++i)
		if (wcscmp(m_shaders[i]->GetPath(), path) == 0 && m_shaders[i]->GetVisibility() == visibility)
			return m_shaders[i];

	m_shaders.push_back(new Shader(path, visibility));
	return m_shaders.back();

}

Texture* Dx12ResourceHandler::GetTexture(LPCWSTR path) {

	for (unsigned int i = 0; i < m_textures.size(); ++i)
		if (wcscmp(m_textures[i]->GetPath(), path) == 0)
			return m_textures[i];

	m_textures.push_back(new Texture(m_device, path));
	return m_textures.back();
}

MeshRenderer* Dx12ResourceHandler::GetMeshRenderer(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, Model* model, std::vector<Shader*>& shaders,
													std::vector<Texture*>& textures, bool hasStaticSampler, std::vector<ConstantBuffer*>& cbs) {

	D3D12_ROOT_SIGNATURE_FLAGS flags{};
	SetFlags(inputLayoutDesc, flags, shaders);

	RootSignature* rs = GetRootSignature(cbs, textures.size(), hasStaticSampler, flags);
	PSO* pso = GetPso(inputLayoutDesc, rs, shaders);

	m_meshRenderers.push_back(new MeshRenderer(m_device, model, pso, cbs, textures));
	return m_meshRenderers.back();
}

bool Dx12ResourceHandler::FindShader(std::vector<Shader*>& shaders, D3D12_SHADER_VISIBILITY visibility) {

	for (unsigned int i = 0; i < shaders.size(); ++i)
		if (shaders[i]->GetVisibility() == visibility)
			return true;

	return false;
}

void Dx12ResourceHandler::SetFlags(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, D3D12_ROOT_SIGNATURE_FLAGS& flags, std::vector<Shader*>& shaders) {

	if (inputLayoutDesc != nullptr)
		flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	if (!FindShader(shaders, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX))
		flags = flags | D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS;
	if (!FindShader(shaders, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL))
		flags = flags | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS;
	if (!FindShader(shaders, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN))
		flags = flags | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;
	if (!FindShader(shaders, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY))
		flags = flags | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	if (!FindShader(shaders, D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL))
		flags = flags | D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;
}