#ifndef _PSO_H_
#define _PSO_H_

#include "../Shader/Shader.h"
#include "../RootSignature/RootSignature.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class PSO
{
	public:

		PSO(Device* device, D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, RootSignature* rootSignature, std::vector<Shader*> shaderTab);
		PSO(const PSO& other);
		PSO(PSO&& other) noexcept;
		~PSO();

		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPso() const;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC GetPsoDesc() const;
		RootSignature* GetRootSignature() const;
		Shader* GetVertexShader() const;
		Shader* GetHullShader() const;
		Shader* GetDomainShader() const;
		Shader* GetGeometryShader() const;
		Shader* GetPixelShader() const;
		std::vector<Shader*> GetShaders() const;
		D3D12_INPUT_LAYOUT_DESC* GetInputLayoutDesc() const;

		PSO& operator=(const PSO& other);
		PSO& operator=(PSO&& other) noexcept;
		bool operator==(const PSO& other) const;
		bool operator!=(const PSO& other) const;

private:

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pso;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_psoDesc;
		RootSignature* m_rootSignature;
		std::vector<Shader*> m_shaders;
		D3D12_INPUT_LAYOUT_DESC* m_inputLayout;
		DXGI_SAMPLE_DESC m_sampleDesc;

};

#endif