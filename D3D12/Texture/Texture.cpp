#include "Texture.h"
#include "../../Loaders/TextureLoader.h"
#include "../../Windows/Windows.h"

using namespace Microsoft::WRL;

Texture::Texture(Device* device, LPCWSTR path) :
    m_path{ path },
    m_textureBuffer { nullptr },
    m_imageData{ nullptr },
    m_srvDesc{ new D3D12_SHADER_RESOURCE_VIEW_DESC()},
    m_textureDesc{},
    m_imageBytesPerRow{},
    m_imageSize{},
    m_defaultHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) },
    m_textureUploadBufferSize{},
    m_uploadHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) },
    m_resourceDescriptor{},
    m_textureData{}
 {

    HRESULT hr;

// Load the image from file
   m_imageSize = textureLoader::LoadImageDataFromFile(&m_imageData, m_textureDesc, path, m_imageBytesPerRow);

    // make sure we have data
    if (m_imageSize <= 0)
    {
        MessageBox(0, L"Texture image size <= 0",
            L"Error", MB_OK);
    }

    hr = device->GetDevice()->CreateCommittedResource(
        &m_defaultHeapProperty,
        D3D12_HEAP_FLAG_NONE,
        &m_textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_textureBuffer));
    if (FAILED(hr))
    {
        MessageBox(0, L"Texture: failed to create default heap",
            L"Error", MB_OK);
    }
    m_textureBuffer->SetName(L"Texture Buffer Resource Heap");

    // this function gets the size an upload buffer needs to be to upload a texture to the gpu.
    // each row must be 256 byte aligned except for the last row, which can just be the size in bytes of the row
    // eg. textureUploadBufferSize = ((((width * numBytesPerPixel) + 255) & ~255) * (height - 1)) + (width * numBytesPerPixel);
    //textureUploadBufferSize = (((imageBytesPerRow + 255) & ~255) * (textureDesc.Height - 1)) + imageBytesPerRow;
    device->GetDevice()->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &m_textureUploadBufferSize);
    m_resourceDescriptor = CD3DX12_RESOURCE_DESC::Buffer(m_textureUploadBufferSize);

    hr = device->GetDevice()->CreateCommittedResource(
        &m_uploadHeapProperty,
        D3D12_HEAP_FLAG_NONE,
        &m_resourceDescriptor,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_textureBufferUploadHeap));
    if (FAILED(hr))
    {
        MessageBox(0, L"Texture: failed to create upload heap",
            L"Error", MB_OK);
    }
    m_textureBufferUploadHeap->SetName(L"Texture Buffer Upload Resource Heap");

    m_textureData.pData = &m_imageData[0]; // pointer to our image data
    m_textureData.RowPitch = m_imageBytesPerRow;
    m_textureData.SlicePitch = m_imageBytesPerRow * m_textureDesc.Height;

    Window::m_resourcesToInit.push_back(ResourcesToInit(m_textureBuffer.Get(), m_textureBufferUploadHeap.Get(), 0, 0, 1, &m_textureData, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

    // now we create a shader resource view (descriptor that points to the texture and describes it)
    m_srvDesc->Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    m_srvDesc->Format = m_textureDesc.Format;
    m_srvDesc->ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    m_srvDesc->Texture2D.MipLevels = 1;

}

Texture::Texture(const Texture& other):
    m_path{other.m_path},
    m_textureBuffer{other.m_textureBuffer},
    m_textureBufferUploadHeap{other.m_textureBufferUploadHeap},
    m_imageData{other.m_imageData},
    m_srvDesc{other.m_srvDesc},
    m_textureDesc{other.m_textureDesc},
    m_imageBytesPerRow{other.m_imageBytesPerRow},
    m_imageSize{other.m_imageSize},
    m_defaultHeapProperty{other.m_defaultHeapProperty},
    m_textureUploadBufferSize{other.m_textureUploadBufferSize},
    m_uploadHeapProperty{other.m_uploadHeapProperty},
    m_resourceDescriptor{other.m_resourceDescriptor},
    m_textureData{other.m_textureData}
{}
Texture::Texture(Texture&& other) noexcept:
    m_path{ other.m_path },
    m_textureBuffer{ other.m_textureBuffer },
    m_textureBufferUploadHeap{ other.m_textureBufferUploadHeap },
    m_imageData{ other.m_imageData },
    m_srvDesc{ other.m_srvDesc },
    m_textureDesc{ other.m_textureDesc },
    m_imageBytesPerRow{ other.m_imageBytesPerRow },
    m_imageSize{ other.m_imageSize },
    m_defaultHeapProperty{ other.m_defaultHeapProperty },
    m_textureUploadBufferSize{ other.m_textureUploadBufferSize },
    m_uploadHeapProperty{ other.m_uploadHeapProperty },
    m_resourceDescriptor{ other.m_resourceDescriptor },
    m_textureData{ other.m_textureData }
{}

Texture::~Texture(){
	
    SAFE_DELETE(m_imageData);
    SAFE_DELETE(m_srvDesc)
}

Texture& Texture::operator=(const Texture& other) {
    m_path = other.m_path;
    m_textureBuffer = other.m_textureBuffer;
    m_textureBufferUploadHeap = other.m_textureBufferUploadHeap;
    m_imageData = other.m_imageData;
    m_srvDesc = other.m_srvDesc;
    m_textureDesc = other.m_textureDesc;
    m_imageBytesPerRow = other.m_imageBytesPerRow;
    m_imageSize = other.m_imageSize;
    m_defaultHeapProperty = other.m_defaultHeapProperty;
    m_textureUploadBufferSize = other.m_textureUploadBufferSize;
    m_uploadHeapProperty = other.m_uploadHeapProperty;
    m_resourceDescriptor = other.m_resourceDescriptor;
    m_textureData = other.m_textureData;

    return *this;
}
Texture& Texture::operator=(Texture&& other) noexcept {
    m_path = other.m_path;
    m_textureBuffer = other.m_textureBuffer;
    m_textureBufferUploadHeap = other.m_textureBufferUploadHeap;
    m_imageData = other.m_imageData;
    m_srvDesc = other.m_srvDesc;
    m_textureDesc = other.m_textureDesc;
    m_imageBytesPerRow = other.m_imageBytesPerRow;
    m_imageSize = other.m_imageSize;
    m_defaultHeapProperty = other.m_defaultHeapProperty;
    m_textureUploadBufferSize = other.m_textureUploadBufferSize;
    m_uploadHeapProperty = other.m_uploadHeapProperty;
    m_resourceDescriptor = other.m_resourceDescriptor;
    m_textureData = other.m_textureData;

    return *this;
}

const LPCWSTR Texture::GetPath() const                              { return m_path; }
ComPtr<ID3D12Resource> Texture::GetTextureBuffer() const            { return m_textureBuffer; }
ComPtr<ID3D12Resource> Texture::GetTextureBufferUploadHeap() const  { return m_textureBufferUploadHeap; }
BYTE* Texture::GetImageData() const                                 { return m_imageData; }
D3D12_SHADER_RESOURCE_VIEW_DESC* Texture::GetSrvDesc() const        { return m_srvDesc; }

bool Texture::operator==(const Texture& other) const {

    return (wcscmp(m_path, other.m_path) == 0);

}

bool Texture::operator!=(const Texture& other) const {

    return (wcscmp(m_path, other.m_path) != 0);

}