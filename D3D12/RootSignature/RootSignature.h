#ifndef _ROOT_SIGNATURE_H_
#define _ROOT_SIGNATURE_H_

#include "../Device/Device.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "../Texture/Texture.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

struct CbComparator {

	D3D12_SHADER_VISIBILITY m_visibility;
	int m_registerNb;

	CbComparator():
		m_visibility{ D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL },
		m_registerNb{ 0 }
	{}
	CbComparator(D3D12_SHADER_VISIBILITY vis, int registerNb) :
		m_visibility{ vis },
		m_registerNb{ registerNb }
	{}
	CbComparator(const CbComparator& other) :
		m_visibility{ other.m_visibility },
		m_registerNb{ other.m_registerNb }
	{}
	CbComparator(CbComparator&& other) noexcept : 
		m_visibility{ other.m_visibility },
		m_registerNb{ other.m_registerNb }
	{}
	CbComparator& operator=(const CbComparator& other) {
		m_visibility = other.m_visibility;
		m_registerNb = other.m_registerNb;

		return *this;
	}
	CbComparator& operator=(CbComparator&& other) noexcept {
		m_visibility = other.m_visibility;
		m_registerNb = other.m_registerNb;

		return *this;
	}
	bool operator==(const CbComparator& other) const {

		return (m_visibility == other.m_visibility) && (m_registerNb == m_registerNb);
	}
	bool operator!=(const CbComparator& other) const {

		return (m_visibility != other.m_visibility) || (m_registerNb != m_registerNb);
	}
};

class RootSignature {

	public:

		RootSignature(Device* device, std::vector<ConstantBuffer*>& cbs, unsigned int texNb, bool hasStaticSampler, D3D12_ROOT_SIGNATURE_FLAGS flags);
		RootSignature(const RootSignature& other);
		RootSignature(RootSignature&& other) noexcept;
		~RootSignature();

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() const;
		unsigned int GetCBNb() const;
		unsigned int GetTextureNb() const;
		unsigned int GetParamNb() const;
		D3D12_ROOT_PARAMETER* GetParameters() const;
		bool HasStaticSampler() const;
		D3D12_STATIC_SAMPLER_DESC GetStaticSampler() const;
		D3D12_ROOT_SIGNATURE_FLAGS GetFlags() const;
		std::vector<CbComparator> GetComparators() const;

		RootSignature& operator=(const RootSignature& other);
		RootSignature& operator=(RootSignature&& other) noexcept;
		bool operator==(const RootSignature& other) const;
		bool operator!=(const RootSignature& other) const;

	private:

		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		ID3DBlob* m_signature;
		unsigned int m_nbCb;
		unsigned int m_nbTex;
		D3D12_ROOT_PARAMETER* m_parameters;
		bool m_hasStaticSampler;
		D3D12_STATIC_SAMPLER_DESC m_staticSampler;
		D3D12_ROOT_SIGNATURE_FLAGS m_flags;
		D3D12_ROOT_DESCRIPTOR* m_rootCBVDescriptor;
		D3D12_DESCRIPTOR_RANGE*  m_descriptorTableRanges;
		D3D12_ROOT_DESCRIPTOR_TABLE m_descriptorTable;
		CD3DX12_ROOT_SIGNATURE_DESC m_rootSignatureDesc;
		std::vector<CbComparator> m_cbComparators;

};


#endif
