#include "Pso.h"

using namespace Microsoft::WRL;

PSO::PSO(Device* device, D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, RootSignature* rootSignature, std::vector<Shader*> shaderTab) :
    m_pso{ nullptr },
    m_psoDesc{},
    m_rootSignature{ rootSignature },
    m_shaders{ shaderTab },
    m_inputLayout{ inputLayoutDesc },
    m_sampleDesc{}
{
    HRESULT hr;

    m_sampleDesc.Count = 1;

    m_psoDesc.InputLayout = *m_inputLayout;
    m_psoDesc.pRootSignature = m_rootSignature->GetRootSignature().Get();
    m_psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    m_psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_psoDesc.SampleDesc = m_sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
    m_psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
    m_psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    m_psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    m_psoDesc.NumRenderTargets = 1;
    m_psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    m_psoDesc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;

    for (unsigned int i = 0; i < m_shaders.size(); ++i) {

        switch (m_shaders[i]->GetVisibility()) {

        case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX: {

            m_psoDesc.VS = m_shaders[i]->GetbyteCode();
            break;
        }

        case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL: {

            m_psoDesc.HS = m_shaders[i]->GetbyteCode();
            break;
        }

        case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN: {

            m_psoDesc.DS = m_shaders[i]->GetbyteCode();
            break;
        }

        case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY: {

            m_psoDesc.GS = m_shaders[i]->GetbyteCode();
            break;
        }

        case D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL: {

            m_psoDesc.PS = m_shaders[i]->GetbyteCode();
            break;
        }

        }
    }

    // create the pso
    hr = device->GetDevice()->CreateGraphicsPipelineState(&m_psoDesc, IID_PPV_ARGS(&m_pso));
    if (FAILED(hr))
    {
        MessageBox(0, L"Couldn't create PSO",
            L"Error", MB_OK);
    }
}

PSO::PSO(const PSO& other):
    m_pso{ other.m_pso },
    m_psoDesc{ other.m_psoDesc },
    m_rootSignature{ other.m_rootSignature },
    m_shaders{ other.m_shaders },
    m_inputLayout{ other.m_inputLayout },
    m_sampleDesc{ other.m_sampleDesc }
{}

PSO::PSO(PSO&& other) noexcept:
    m_pso{ other.m_pso },
    m_psoDesc{ other.m_psoDesc },
    m_rootSignature{ other.m_rootSignature },
    m_shaders{ other.m_shaders },
    m_inputLayout{ other.m_inputLayout },
    m_sampleDesc{ other.m_sampleDesc }
{}

PSO::~PSO()
{}

ComPtr<ID3D12PipelineState> PSO::GetPso() const { return m_pso; }
D3D12_GRAPHICS_PIPELINE_STATE_DESC PSO::GetPsoDesc() const { return m_psoDesc; }
RootSignature* PSO::GetRootSignature() const { return m_rootSignature; }
Shader* PSO::GetVertexShader() const {

    for (unsigned int i = 0; i < m_shaders.size(); ++i)
        if (m_shaders[i]->GetVisibility() == D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX)
            return m_shaders[i];
    return nullptr;
}
Shader* PSO::GetHullShader() const {

    for (unsigned int i = 0; i < m_shaders.size(); ++i)
        if (m_shaders[i]->GetVisibility() == D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL)
            return m_shaders[i];
    return nullptr;
}
Shader* PSO::GetDomainShader() const {

    for (unsigned int i = 0; i < m_shaders.size(); ++i)
        if (m_shaders[i]->GetVisibility() == D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN)
            return m_shaders[i];
    return nullptr;
}
Shader* PSO::GetGeometryShader() const {

    for (unsigned int i = 0; i < m_shaders.size(); ++i)
        if (m_shaders[i]->GetVisibility() == D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY)
            return m_shaders[i];
    return nullptr;
}
Shader* PSO::GetPixelShader() const {

    for (unsigned int i = 0; i < m_shaders.size(); ++i)
        if (m_shaders[i]->GetVisibility() == D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL)
            return m_shaders[i];
    return nullptr;
}

std::vector<Shader*> PSO::GetShaders() const { return m_shaders; }
D3D12_INPUT_LAYOUT_DESC* PSO::GetInputLayoutDesc() const { return m_inputLayout; }


PSO& PSO::operator=(const PSO& other) {
    m_pso = other.m_pso;
    m_psoDesc = other.m_psoDesc;
    m_rootSignature = other.m_rootSignature;
    m_shaders = other.m_shaders;
    m_inputLayout = other.m_inputLayout;
    m_sampleDesc = other.m_sampleDesc;

    return *this;
}

PSO& PSO::operator=(PSO&& other) noexcept {
    m_pso = other.m_pso;
    m_psoDesc = other.m_psoDesc;
    m_rootSignature = other.m_rootSignature;
    m_shaders = other.m_shaders;
    m_inputLayout = other.m_inputLayout;
    m_sampleDesc = other.m_sampleDesc;

    return *this;
}

bool PSO::operator==(const PSO& other) const {

    if (m_inputLayout->NumElements == other.m_inputLayout->NumElements) {
        for (unsigned int i = 0; i < m_inputLayout->NumElements; ++i)
            if (m_inputLayout->pInputElementDescs[i].AlignedByteOffset != other.m_inputLayout->pInputElementDescs[i].AlignedByteOffset ||
                m_inputLayout->pInputElementDescs[i].Format != other.m_inputLayout->pInputElementDescs[i].Format ||
                m_inputLayout->pInputElementDescs[i].InputSlot != other.m_inputLayout->pInputElementDescs[i].InputSlot ||
                m_inputLayout->pInputElementDescs[i].InputSlotClass != other.m_inputLayout->pInputElementDescs[i].InputSlotClass ||
                m_inputLayout->pInputElementDescs[i].InstanceDataStepRate != other.m_inputLayout->pInputElementDescs[i].InstanceDataStepRate ||
                m_inputLayout->pInputElementDescs[i].SemanticIndex != other.m_inputLayout->pInputElementDescs[i].SemanticIndex ||
                m_inputLayout->pInputElementDescs[i].SemanticName != other.m_inputLayout->pInputElementDescs[i].SemanticName)
                return false;
        return (*m_rootSignature == *other.m_rootSignature) && (m_shaders == other.m_shaders);
    }

    return false;

}

bool PSO::operator!=(const PSO& other) const {

    if (m_inputLayout->NumElements == other.m_inputLayout->NumElements) {
        for (unsigned int i = 0; i < m_inputLayout->NumElements; ++i)
            if (m_inputLayout->pInputElementDescs[i].AlignedByteOffset != other.m_inputLayout->pInputElementDescs[i].AlignedByteOffset ||
                m_inputLayout->pInputElementDescs[i].Format != other.m_inputLayout->pInputElementDescs[i].Format ||
                m_inputLayout->pInputElementDescs[i].InputSlot != other.m_inputLayout->pInputElementDescs[i].InputSlot ||
                m_inputLayout->pInputElementDescs[i].InputSlotClass != other.m_inputLayout->pInputElementDescs[i].InputSlotClass ||
                m_inputLayout->pInputElementDescs[i].InstanceDataStepRate != other.m_inputLayout->pInputElementDescs[i].InstanceDataStepRate ||
                m_inputLayout->pInputElementDescs[i].SemanticIndex != other.m_inputLayout->pInputElementDescs[i].SemanticIndex ||
                m_inputLayout->pInputElementDescs[i].SemanticName != other.m_inputLayout->pInputElementDescs[i].SemanticName)
                return true;
        return (*m_rootSignature != *other.m_rootSignature) || (m_shaders != other.m_shaders);
    }

    return true;
}