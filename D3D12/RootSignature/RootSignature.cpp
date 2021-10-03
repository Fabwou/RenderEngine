#include "RootSignature.h"

using namespace Microsoft::WRL;

RootSignature::RootSignature(Device* device, std::vector<ConstantBuffer*>& cbs, unsigned int texNb, bool hasStaticSampler, D3D12_ROOT_SIGNATURE_FLAGS flags) :
	m_rootSignature{ nullptr },
	m_signature{ nullptr },
	m_nbCb{ static_cast<unsigned int>(cbs.size()) },
	m_nbTex{ texNb },
	m_hasStaticSampler{ hasStaticSampler },
	m_flags{ flags },
	m_parameters{},
	m_rootCBVDescriptor{ new D3D12_ROOT_DESCRIPTOR[m_nbCb] },
	m_descriptorTableRanges{ new D3D12_DESCRIPTOR_RANGE[1] },
	m_descriptorTable{},
	m_rootSignatureDesc{},
	m_cbComparators{}
{
	m_cbComparators.reserve(cbs.size());
	for (unsigned int i = 0; i < m_nbCb; ++i)
		m_cbComparators.push_back({ cbs[i]->GetShaderVisibility(), cbs[i]->GetRegisterNb() });

	if (m_nbTex > 0)
		m_parameters = new D3D12_ROOT_PARAMETER[m_nbCb + 1];
	else
		m_parameters = new D3D12_ROOT_PARAMETER[m_nbCb];

	HRESULT hr; 

	for (unsigned int i = 0; i < m_nbCb; ++i) {
		// create a root descriptor, which explains where to find the data for this root parameter
		m_rootCBVDescriptor[i].RegisterSpace = 0;
		m_rootCBVDescriptor[i].ShaderRegister = cbs[i]->GetRegisterNb();

		// create a root parameter and fill it out
		m_parameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_parameters[i].Descriptor = m_rootCBVDescriptor[i];
		m_parameters[i].ShaderVisibility = cbs[i]->GetShaderVisibility();
	}

	if (m_nbTex > 0) {
		// create a descriptor range (descriptor table) and fill it out
	// this is a range of descriptors inside a descriptor heap
		 // only one range right now
		m_descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // this is a range of shader resource views (descriptors)
		m_descriptorTableRanges[0].NumDescriptors = m_nbTex; // we only have one texture right now, so the range is only 1
		m_descriptorTableRanges[0].BaseShaderRegister = 0; // start index of the shader registers in the range
		m_descriptorTableRanges[0].RegisterSpace = 0; // space 0. can usually be zero
		m_descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // this appends the range to the end of the root signature descriptor tables

		// create a descriptor table
		m_descriptorTable.NumDescriptorRanges = 1; // we only have one range
		m_descriptorTable.pDescriptorRanges = &m_descriptorTableRanges[0]; // the pointer to the beginning of our ranges array

		// fill out the parameter for our descriptor table. Remember it's a good idea to sort parameters by frequency of change. Our constant
	// buffer will be changed multiple times per frame, while our descriptor table will not be changed at all (in this tutorial)
		m_parameters[m_nbCb].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
		m_parameters[m_nbCb].DescriptorTable = m_descriptorTable; // this is our descriptor table for this root parameter
		m_parameters[m_nbCb].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // our pixel shader will be the only shader accessing this parameter for now
	}

	m_staticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	m_staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	m_staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	m_staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	m_staticSampler.MipLODBias = 0;
	m_staticSampler.MaxAnisotropy = 0;
	m_staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	m_staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	m_staticSampler.MinLOD = 0.0f;
	m_staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	m_staticSampler.ShaderRegister = 0;
	m_staticSampler.RegisterSpace = 0;
	m_staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	if(m_nbTex > 0)
		m_rootSignatureDesc.Init(m_nbCb + 1,
								m_parameters,
								static_cast<unsigned int>(m_hasStaticSampler),
								&m_staticSampler,
								flags);
	else
		m_rootSignatureDesc.Init(m_nbCb,
			m_parameters,
			static_cast<unsigned int>(m_hasStaticSampler),
			&m_staticSampler,
			flags);

	hr = D3D12SerializeRootSignature(&m_rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &m_signature, nullptr);
	if (FAILED(hr))
		MessageBox(0, L"Failed to serialize root signature",
			L"Error", MB_OK);

	hr = device->GetDevice()->CreateRootSignature(0, m_signature->GetBufferPointer(), m_signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	if (FAILED(hr))
		MessageBox(0, L"Failed to create root signature",
			L"Error", MB_OK);

}

RootSignature::RootSignature(const RootSignature& other):
	m_rootSignature{ other.m_rootSignature },
	m_signature{ other.m_signature },
	m_nbCb{ other.m_nbCb },
	m_nbTex{ other.m_nbTex },
	m_hasStaticSampler{ other.m_hasStaticSampler },
	m_staticSampler{ other.m_staticSampler },
	m_flags{ other.m_flags },
	m_parameters{other.m_parameters},
	m_rootCBVDescriptor{ other.m_rootCBVDescriptor },
	m_descriptorTableRanges{ other.m_descriptorTableRanges },
	m_descriptorTable{ other.m_descriptorTable },
	m_rootSignatureDesc{other.m_rootSignatureDesc},
	m_cbComparators{other.m_cbComparators}
{}

RootSignature::RootSignature(RootSignature&& other) noexcept:
	m_rootSignature{ other.m_rootSignature },
	m_signature{ other.m_signature },
	m_nbCb{ other.m_nbCb },
	m_nbTex{ other.m_nbTex },
	m_hasStaticSampler{ other.m_hasStaticSampler },
	m_staticSampler{ other.m_staticSampler },
	m_flags{ other.m_flags },
	m_parameters{ other.m_parameters },
	m_rootCBVDescriptor{ other.m_rootCBVDescriptor },
	m_descriptorTableRanges{ other.m_descriptorTableRanges },
	m_descriptorTable{ other.m_descriptorTable },
	m_rootSignatureDesc{ other.m_rootSignatureDesc },
	m_cbComparators{ other.m_cbComparators }
{}

RootSignature::~RootSignature() {

	SAFE_DELETE(m_parameters);
	SAFE_DELETE_ARRAY(m_rootCBVDescriptor);
	SAFE_DELETE_ARRAY(m_descriptorTableRanges);
	m_cbComparators.clear();
}

ComPtr<ID3D12RootSignature> RootSignature::GetRootSignature() const			{ return m_rootSignature; }
unsigned int RootSignature::GetCBNb() const									{ return m_nbCb; }
unsigned int RootSignature::GetTextureNb() const							{ return m_nbTex; }
unsigned int RootSignature::GetParamNb() const								{ return m_nbCb + m_nbTex; }
D3D12_ROOT_PARAMETER* RootSignature::GetParameters() const					{ return m_parameters; }
bool RootSignature::HasStaticSampler() const								{ return m_hasStaticSampler; }
D3D12_STATIC_SAMPLER_DESC RootSignature::GetStaticSampler() const			{ return m_staticSampler; }
D3D12_ROOT_SIGNATURE_FLAGS RootSignature::GetFlags() const					{ return m_flags; }
std::vector<CbComparator> RootSignature::GetComparators() const				{ return m_cbComparators; }

RootSignature& RootSignature::operator=(const RootSignature& other) {
	m_rootSignature = other.m_rootSignature;
	m_signature = other.m_signature;
	m_nbCb = other.m_nbCb;
	m_nbTex = other.m_nbTex;
	m_hasStaticSampler = other.m_hasStaticSampler;
	m_staticSampler = other.m_staticSampler;
	m_flags = other.m_flags;
	m_parameters = other.m_parameters;
	m_rootCBVDescriptor = other.m_rootCBVDescriptor;
	m_descriptorTableRanges = other.m_descriptorTableRanges;
	m_descriptorTable = other.m_descriptorTable;
	m_rootSignatureDesc = other.m_rootSignatureDesc;
	m_cbComparators = other.m_cbComparators;

	return *this;
}
RootSignature& RootSignature::operator=(RootSignature&& other) noexcept {
	m_rootSignature = other.m_rootSignature;
	m_signature = other.m_signature;
	m_nbCb = other.m_nbCb;
	m_nbTex = other.m_nbTex;
	m_hasStaticSampler = other.m_hasStaticSampler;
	m_staticSampler = other.m_staticSampler;
	m_flags = other.m_flags;
	m_parameters = other.m_parameters;
	m_rootCBVDescriptor = other.m_rootCBVDescriptor;
	m_descriptorTableRanges = other.m_descriptorTableRanges;
	m_descriptorTable = other.m_descriptorTable;
	m_rootSignatureDesc = other.m_rootSignatureDesc;
	m_cbComparators = other.m_cbComparators;

	return *this;
}

bool RootSignature::operator==(const RootSignature& other) const {

	if (m_nbCb == other.m_nbCb) {
		for (unsigned int i = 0; i < m_cbComparators.size(); ++i)
			if (m_cbComparators[i] != other.m_cbComparators[i])
				return false;
	}
	else
		return false;
	return (m_nbTex == other.m_nbTex) && (m_hasStaticSampler == other.m_hasStaticSampler) && (m_flags == other.m_flags);

}
bool RootSignature::operator!=(const RootSignature& other) const {

	if (m_nbCb == other.m_nbCb) {
		for (unsigned int i = 0; i < m_cbComparators.size(); ++i)
			if (m_cbComparators[i] != other.m_cbComparators[i])
				return true;
	}
	else
		return true;
	return (m_nbTex != other.m_nbTex) || (m_hasStaticSampler != other.m_hasStaticSampler) || (m_flags != other.m_flags);

}