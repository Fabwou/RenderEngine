#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "../Device/Device.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Texture {

public:

	Texture(Device* device, LPCWSTR path);
	Texture(const Texture& other);
	Texture(Texture&& other) noexcept;
	~Texture();

	const LPCWSTR GetPath() const;
	Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureBuffer() const;
	Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureBufferUploadHeap() const;
	BYTE* GetImageData() const;
	D3D12_SHADER_RESOURCE_VIEW_DESC* GetSrvDesc() const;

	Texture& operator=(const Texture& other);
	Texture& operator=(Texture&& other) noexcept;
	bool operator==(const Texture& other) const;
	bool operator!=(const Texture& other) const;

private:

	LPCWSTR m_path;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_textureBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_textureBufferUploadHeap;
	BYTE* m_imageData;
	D3D12_SHADER_RESOURCE_VIEW_DESC* m_srvDesc;
	D3D12_RESOURCE_DESC m_textureDesc;
	int m_imageBytesPerRow;
	int m_imageSize;
	CD3DX12_HEAP_PROPERTIES m_defaultHeapProperty;
	UINT64 m_textureUploadBufferSize;
	CD3DX12_HEAP_PROPERTIES m_uploadHeapProperty;
	CD3DX12_RESOURCE_DESC m_resourceDescriptor;
	D3D12_SUBRESOURCE_DATA m_textureData;
};


#endif