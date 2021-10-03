#ifndef _CONSTANT_BUFFER_H_
#define _CONSTANT_BUFFER_H_

#include "../Device/Device.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class ConstantBuffer
{
public:

	template<typename T>
	ConstantBuffer(Device* device, T* data, int sizeofData, int registerNb, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL) :
		m_device{ device },
		m_bufferSize{ device->GetFrameBufferSize() },
		m_readRange{ 0, 0 },
		m_uploadHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) },
		m_cbResourceDesc{ CD3DX12_RESOURCE_DESC::Buffer(D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT) } // size of the resource heap. Must be a multiple of 64KB for single
	{
		Initialize(data, sizeofData, registerNb, shaderVisibility);
	}

	ConstantBuffer(const ConstantBuffer& other) :
		m_device{ other.m_device },
	 	m_bufferSize{ other.m_bufferSize },
		m_dataSize{ other.m_dataSize },
		m_registerNb{ other.m_registerNb },
		m_shaderVisibility{ other.m_shaderVisibility },
		m_cbUploadHeap{ other.m_cbUploadHeap },
		m_cbGPUAddress{ other.m_cbGPUAddress },
		m_readRange{ other.m_readRange },
		m_uploadHeapProperty{ other.m_uploadHeapProperty },
		m_cbResourceDesc{ other.m_cbResourceDesc }
	{}

	ConstantBuffer(ConstantBuffer&& other) noexcept :
		m_device{ other.m_device },
	 	m_bufferSize{ other.m_bufferSize },
		m_dataSize{ other.m_dataSize },
		m_registerNb{ other.m_registerNb },
		m_shaderVisibility{ other.m_shaderVisibility },
		m_cbUploadHeap{ other.m_cbUploadHeap },
		m_cbGPUAddress{ other.m_cbGPUAddress },
		m_readRange{ other.m_readRange },
		m_uploadHeapProperty{ other.m_uploadHeapProperty },
		m_cbResourceDesc{ other.m_cbResourceDesc }
	{}

	~ConstantBuffer()
	{
		SAFE_DELETE_ARRAY(m_cbUploadHeap);
		SAFE_DELETE_ARRAY(m_cbGPUAddress);
	}

	template<typename T>
	void Initialize(T* data, int sizeofData, int registerNb, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL)
	{
		if (!data)
			return;
		m_dataSize = sizeofData;
		m_shaderVisibility = shaderVisibility;
		m_registerNb = registerNb;
		m_cbUploadHeap = new Microsoft::WRL::ComPtr<ID3D12Resource> [m_bufferSize]();
		m_cbGPUAddress = new UINT8 * [m_bufferSize];

		HRESULT hr;

		for (int i{ 0 }; i < m_bufferSize; ++i)
		{
			hr = m_device->GetDevice()->CreateCommittedResource(
				&m_uploadHeapProperty,
				D3D12_HEAP_FLAG_NONE,
				&m_cbResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_cbUploadHeap[i]));
			if (FAILED(hr))
			{
				MessageBox(0, L"ConstantBuffer: failed to create upload heap",
					L"Error", MB_OK);
			}
			m_cbUploadHeap[i]->SetName(L"Constant Buffer Upload Resource Heap");
			hr = m_cbUploadHeap[i]->Map(0, &m_readRange, reinterpret_cast<void**>(&m_cbGPUAddress[i]));
			if (FAILED(hr))
			{
				MessageBox(0, L"ConstantBuffer: failed to copy data",
					L"Error", MB_OK);
			}
			memcpy(m_cbGPUAddress[i], data, sizeofData);
		}

	}

	template<typename T>
	void Update(T* data, int sizeofData, int frameIndex)
	{
		if (data) {
			m_dataSize = sizeofData;
			memcpy(m_cbGPUAddress[frameIndex], data, sizeofData);
		}
	}

	Device* GetDevice() const { return m_device; }
	int GetDataSize() const { return m_dataSize; }
	Microsoft::WRL::ComPtr<ID3D12Resource>* GetCbUploadHeap() const { return m_cbUploadHeap; }
	UINT8** GetGPUAddress() const { return m_cbGPUAddress; }
	int GetRegisterNb() const { return m_registerNb; }
	D3D12_SHADER_VISIBILITY GetShaderVisibility() const { return m_shaderVisibility; }

	ConstantBuffer& operator=(const ConstantBuffer& other) {
		m_device = other.m_device;
	 	m_bufferSize = other.m_bufferSize;
		m_dataSize = other.m_dataSize;
		m_registerNb = other.m_registerNb;
		m_shaderVisibility = other.m_shaderVisibility;
		m_cbUploadHeap = other.m_cbUploadHeap;
		m_cbGPUAddress = other.m_cbGPUAddress;
		m_readRange = other.m_readRange;
		m_uploadHeapProperty = other.m_uploadHeapProperty;
		m_cbResourceDesc = other.m_cbResourceDesc;

		return *this;
	}

	ConstantBuffer& operator=(ConstantBuffer&& other) noexcept {
		m_device = other.m_device;
	 	m_bufferSize = other.m_bufferSize;
		m_dataSize = other.m_dataSize;
		m_registerNb = other.m_registerNb;
		m_shaderVisibility = other.m_shaderVisibility;
		m_cbUploadHeap = other.m_cbUploadHeap;
		m_cbGPUAddress = other.m_cbGPUAddress;
		m_readRange = other.m_readRange;
		m_uploadHeapProperty = other.m_uploadHeapProperty;
		m_cbResourceDesc = other.m_cbResourceDesc;

		return *this;
	}

	bool operator==(const ConstantBuffer& other) const {

		return (m_cbGPUAddress == other.m_cbGPUAddress);
	}
	bool operator!=(const ConstantBuffer& other) const {

		return (m_cbGPUAddress != other.m_cbGPUAddress);
	}

private:

	Device* m_device;
	int m_dataSize;
	int m_registerNb;
	int m_bufferSize;
	D3D12_SHADER_VISIBILITY m_shaderVisibility;
	Microsoft::WRL::ComPtr<ID3D12Resource>* m_cbUploadHeap;
	UINT8** m_cbGPUAddress;
	CD3DX12_RANGE m_readRange;
	CD3DX12_HEAP_PROPERTIES m_uploadHeapProperty;
	CD3DX12_RESOURCE_DESC m_cbResourceDesc;
};

#endif
