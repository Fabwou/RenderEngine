#ifndef _DX12RESOURCEHANDLER_H_
#define _DX12RESOURCEHANDLER_H_

#include "../../Mesh/MeshRenderer/MeshRenderer.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Dx12ResourceHandler {

public:

	Dx12ResourceHandler(Device* device);
	~Dx12ResourceHandler();

	RootSignature* GetRootSignature(std::vector<ConstantBuffer*>& cbs, unsigned int texNb, bool hasStaticSampler, D3D12_ROOT_SIGNATURE_FLAGS flags);

	PSO* GetPso(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, RootSignature* rootSignature, std::vector<Shader*>& shaderTab);

	Shader* GetShader(LPCWSTR path, D3D12_SHADER_VISIBILITY visibility);
	Texture* GetTexture(LPCWSTR path);

	MeshRenderer* GetMeshRenderer(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, Model* model, std::vector<Shader*>& shaders,
									std::vector<Texture*>& textures, bool hasStaticSampler, std::vector<ConstantBuffer*>& cbs);

private:

	Device* m_device;

	std::vector<RootSignature*> m_rootSignatures;
	std::vector<PSO*> m_psos;
	std::vector<Shader*> m_shaders;
	std::vector<Texture*> m_textures;
	std::vector<MeshRenderer*> m_meshRenderers;

	bool FindShader(std::vector<Shader*>& shaders, D3D12_SHADER_VISIBILITY visibility);
	void SetFlags(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, D3D12_ROOT_SIGNATURE_FLAGS& flags, std::vector<Shader*>& shaders);


};

#endif // !_DX12RESOURCEHANDLER_H_
