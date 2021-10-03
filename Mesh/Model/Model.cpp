#include "Model.h"
#include "../../Windows/Windows.h"

using namespace Microsoft::WRL;

Model::Model(Device* device, LPCWSTR name, std::vector<Vertex> vertices, std::vector<int> indices) :
	m_name{},
	m_verticesSize{ static_cast<unsigned int>(vertices.size()) },
	m_indicesSize{ static_cast<unsigned int>(indices.size()) },
	m_vertices{ vertices },
	m_indices{ indices },
	m_verticesSizeInBytes{ static_cast<unsigned int>(sizeof(Vertex)) * m_verticesSize },
	m_indicesSizeInBytes{ static_cast<unsigned int>(sizeof(int)) * m_indicesSize },
	m_vertexBuffer{ nullptr },
	m_vertexBufferView{ new D3D12_VERTEX_BUFFER_VIEW },
	m_indexBuffer{ nullptr },
	m_indexBufferView{ new D3D12_INDEX_BUFFER_VIEW },
	m_defaultHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) },
	m_verticesBufferSizeDesc{ CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT64>(m_verticesSizeInBytes)) },
	m_vBufferUploadHeap{ nullptr },
	m_uploadHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) },
	m_iBufferUploadHeap{ nullptr },
	m_vertexData{},
	m_indexData{}
{

	const size_t size = wcslen(name) + 1;
	m_name.reserve(size);
	for (unsigned int i = 0; i < size; ++i)
		m_name.push_back(name[i]);

	HRESULT hr;
	unsigned int i = 0;

	// create default heap
	hr = device->GetDevice()->CreateCommittedResource(
		&m_defaultHeapProperty, // a default heap
		D3D12_HEAP_FLAG_NONE,
		&m_verticesBufferSizeDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create default heap for vertices",
			L"Error", MB_OK);
	}

	m_vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

	// create upload heap
	hr = device->GetDevice()->CreateCommittedResource(
		&m_uploadHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&m_verticesBufferSizeDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vBufferUploadHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create upload heap for vertices",
			L"Error", MB_OK);
	}
	m_vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	m_vertexData.pData = reinterpret_cast<BYTE*>(&m_vertices[0]);// pointer to our vertex array
	m_vertexData.RowPitch = m_verticesSizeInBytes; // size of all our triangle vertex data
	m_vertexData.SlicePitch = m_verticesSizeInBytes; // also the size of our triangle vertex data

	// we are now creating a command with the command list to copy the data from
	// the upload heap to the default heap
	Window::m_resourcesToInit.push_back(ResourcesToInit(m_vertexBuffer.Get(), m_vBufferUploadHeap.Get(), 0, 0, 1, &m_vertexData, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	m_vertexBufferView->BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView->StrideInBytes = sizeof(Vertex);
	m_vertexBufferView->SizeInBytes = m_verticesSizeInBytes;

	// create default heap to hold index buffer

	CD3DX12_RESOURCE_DESC indicesBufferSizeDesc = CD3DX12_RESOURCE_DESC::Buffer(m_indicesSizeInBytes);
	hr = device->GetDevice()->CreateCommittedResource(
		&m_defaultHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&indicesBufferSizeDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create default heap for indices",
			L"Error", MB_OK);
	}
	// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
	m_indexBuffer->SetName(L"Index Buffer Resource Heap");

	// create upload heap to upload index buffer
	hr = device->GetDevice()->CreateCommittedResource(
		&m_uploadHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&indicesBufferSizeDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_iBufferUploadHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create upload heap for indices",
			L"Error", MB_OK);
	}
	m_iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	m_indexData.pData = reinterpret_cast<BYTE*>(&m_indices[0]); // pointer to our index array
	m_indexData.RowPitch = m_indicesSizeInBytes; // size of all our index buffer
	m_indexData.SlicePitch = m_indicesSizeInBytes; // also the size of our index buffer

	// we are now creating a command with the command list to copy the data from
	// the upload heap to the default heap
	Window::m_resourcesToInit.push_back(ResourcesToInit(m_indexBuffer.Get(), m_iBufferUploadHeap.Get(), 0, 0, 1, &m_indexData, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	m_indexBufferView->BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView->Format = DXGI_FORMAT_R32_UINT;
	m_indexBufferView->SizeInBytes = m_indicesSizeInBytes;
}

Model::Model(Device* device, const char* name, std::vector<Vertex> vertices, std::vector<int> indices) :
	m_name{},
	m_verticesSize{ static_cast<unsigned int>(vertices.size()) },
	m_indicesSize{ static_cast<unsigned int>(indices.size()) },
	m_vertices{ vertices },
	m_indices{ indices },
	m_verticesSizeInBytes{ static_cast<unsigned int>(sizeof(Vertex)) * m_verticesSize },
	m_indicesSizeInBytes{ static_cast<unsigned int>(sizeof(int)) * m_indicesSize },
	m_vertexBuffer{ nullptr },
	m_vertexBufferView{ new D3D12_VERTEX_BUFFER_VIEW },
	m_indexBuffer{ nullptr },
	m_indexBufferView{ new D3D12_INDEX_BUFFER_VIEW },
	m_defaultHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) },
	m_verticesBufferSizeDesc{ CD3DX12_RESOURCE_DESC::Buffer(static_cast<UINT64>(m_verticesSizeInBytes)) },
	m_vBufferUploadHeap{ nullptr },
	m_uploadHeapProperty{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD) },
	m_iBufferUploadHeap{ nullptr },
	m_vertexData{},
	m_indexData{}
{

	const size_t size = strlen(name) + 1;
	size_t returnValue = 0;
	m_name.resize(size, L'h');
	errno_t erTest = mbstowcs_s(&returnValue, &m_name[0], size, name, size - 1);

	HRESULT hr;
	unsigned int i = 0;

	// create default heap
	hr = device->GetDevice()->CreateCommittedResource(
		&m_defaultHeapProperty, // a default heap
		D3D12_HEAP_FLAG_NONE,
		&m_verticesBufferSizeDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_vertexBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create default heap for vertices",
			L"Error", MB_OK);
	}

	m_vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

	// create upload heap
	hr = device->GetDevice()->CreateCommittedResource(
		&m_uploadHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&m_verticesBufferSizeDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vBufferUploadHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create upload heap for vertices",
			L"Error", MB_OK);
	}
	m_vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	m_vertexData.pData = reinterpret_cast<BYTE*>(&m_vertices[0]);// pointer to our vertex array
	m_vertexData.RowPitch = m_verticesSizeInBytes; // size of all our triangle vertex data
	m_vertexData.SlicePitch = m_verticesSizeInBytes; // also the size of our triangle vertex data

	// we are now creating a command with the command list to copy the data from
	// the upload heap to the default heap
	Window::m_resourcesToInit.push_back(ResourcesToInit(m_vertexBuffer.Get(), m_vBufferUploadHeap.Get(), 0, 0, 1, &m_vertexData, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	m_vertexBufferView->BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	m_vertexBufferView->StrideInBytes = sizeof(Vertex);
	m_vertexBufferView->SizeInBytes = m_verticesSizeInBytes;

	// create default heap to hold index buffer

	CD3DX12_RESOURCE_DESC indicesBufferSizeDesc = CD3DX12_RESOURCE_DESC::Buffer(m_indicesSizeInBytes);
	hr = device->GetDevice()->CreateCommittedResource(
		&m_defaultHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&indicesBufferSizeDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create default heap for indices",
			L"Error", MB_OK);
	}
	// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
	m_indexBuffer->SetName(L"Index Buffer Resource Heap");

	// create upload heap to upload index buffer
	hr = device->GetDevice()->CreateCommittedResource(
		&m_uploadHeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&indicesBufferSizeDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_iBufferUploadHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Model: failed to create upload heap for indices",
			L"Error", MB_OK);
	}
	m_iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

	// store vertex buffer in upload heap
	m_indexData.pData = reinterpret_cast<BYTE*>(&m_indices[0]); // pointer to our index array
	m_indexData.RowPitch = m_indicesSizeInBytes; // size of all our index buffer
	m_indexData.SlicePitch = m_indicesSizeInBytes; // also the size of our index buffer

	// we are now creating a command with the command list to copy the data from
	// the upload heap to the default heap
	Window::m_resourcesToInit.push_back(ResourcesToInit(m_indexBuffer.Get(), m_iBufferUploadHeap.Get(), 0, 0, 1, &m_indexData, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

	// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
	m_indexBufferView->BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_indexBufferView->Format = DXGI_FORMAT_R32_UINT;
	m_indexBufferView->SizeInBytes = m_indicesSizeInBytes;
}

Model::Model(const Model& other) :
	m_name{ other.m_name },
	m_vertices{ other.m_vertices },
	m_indices{ other.m_indices },
	m_verticesSize{ other.m_verticesSize },
	m_verticesSizeInBytes{ other.m_verticesSizeInBytes },
	m_indicesSize{ other.m_indicesSize },
	m_indicesSizeInBytes{ other.m_indicesSizeInBytes },
	m_vertexBuffer{ other.m_vertexBuffer },
	m_vertexBufferView{ other.m_vertexBufferView },
	m_indexBuffer{ other.m_indexBuffer },
	m_indexBufferView{ other.m_indexBufferView },
	m_defaultHeapProperty{ other.m_defaultHeapProperty },
	m_verticesBufferSizeDesc{ other.m_verticesBufferSizeDesc },
	m_vBufferUploadHeap{ other.m_vBufferUploadHeap },
	m_uploadHeapProperty{ other.m_uploadHeapProperty },
	m_iBufferUploadHeap{ other.m_iBufferUploadHeap },
	m_vertexData{ other.m_vertexData },
	m_indexData{ other.m_indexData }
{}

Model::Model(Model&& other) noexcept :
	m_name{ other.m_name },
	m_vertices{ other.m_vertices },
	m_indices{ other.m_indices },
	m_verticesSize{ other.m_verticesSize },
	m_verticesSizeInBytes{ other.m_verticesSizeInBytes },
	m_indicesSize{ other.m_indicesSize },
	m_indicesSizeInBytes{ other.m_indicesSizeInBytes },
	m_vertexBuffer{ other.m_vertexBuffer },
	m_vertexBufferView{ other.m_vertexBufferView },
	m_indexBuffer{ other.m_indexBuffer },
	m_indexBufferView{ other.m_indexBufferView },
	m_defaultHeapProperty{ other.m_defaultHeapProperty },
	m_verticesBufferSizeDesc{ other.m_verticesBufferSizeDesc },
	m_vBufferUploadHeap{ other.m_vBufferUploadHeap },
	m_uploadHeapProperty{ other.m_uploadHeapProperty },
	m_iBufferUploadHeap{ other.m_iBufferUploadHeap },
	m_vertexData{ other.m_vertexData },
	m_indexData{ other.m_indexData }
{}

Model::~Model()
{
	SAFE_DELETE(m_vertexBufferView);
	SAFE_DELETE(m_indexBufferView);
	m_name.clear();
}

const wchar_t* Model::GetPathPtr() const { return &m_name[0]; }

std::vector<Vertex> Model::GetVertices() const { return m_vertices; }
std::vector<int> Model::GetIndices() const { return m_indices; }

unsigned int Model::GetVerticesSize() const { return m_verticesSize; }
unsigned int Model::GetVerticesSizeInBytes() const { return m_verticesSizeInBytes; }
unsigned int Model::GetIndicesSize() const { return m_indicesSize; }
unsigned int Model::GetIndicesSizeInBytes() const { return m_indicesSizeInBytes; }

ComPtr<ID3D12Resource> Model::GetVertexBuffer() const { return m_vertexBuffer; }
D3D12_VERTEX_BUFFER_VIEW* Model::GetVertexBufferView() const { return m_vertexBufferView; }
ComPtr<ID3D12Resource> Model::GetIndexBuffer() const { return m_indexBuffer; }
D3D12_INDEX_BUFFER_VIEW* Model::GetIndexBufferView() const { return m_indexBufferView; }

Model& Model::operator=(const Model& other){
	m_name = other.m_name;
	m_vertices = other.m_vertices;
	m_indices = other.m_indices;
	m_verticesSize = other.m_verticesSize;
	m_verticesSizeInBytes = other.m_verticesSizeInBytes;
	m_indicesSize = other.m_indicesSize;
	m_indicesSizeInBytes = other.m_indicesSizeInBytes;
	m_vertexBuffer = other.m_vertexBuffer;
	m_vertexBufferView = other.m_vertexBufferView;
	m_indexBuffer = other.m_indexBuffer;
	m_indexBufferView = other.m_indexBufferView;
	m_defaultHeapProperty = other.m_defaultHeapProperty;
	m_verticesBufferSizeDesc = other.m_verticesBufferSizeDesc;
	m_vBufferUploadHeap = other.m_vBufferUploadHeap;
	m_uploadHeapProperty = other.m_uploadHeapProperty;
	m_iBufferUploadHeap = other.m_iBufferUploadHeap;
	m_vertexData = other.m_vertexData;
	m_indexData = other.m_indexData;

	return *this;
}
Model& Model::operator=(Model&& other) noexcept {
	m_name = other.m_name;
	m_vertices = other.m_vertices;
	m_indices = other.m_indices;
	m_verticesSize = other.m_verticesSize;
	m_verticesSizeInBytes = other.m_verticesSizeInBytes;
	m_indicesSize = other.m_indicesSize;
	m_indicesSizeInBytes = other.m_indicesSizeInBytes;
	m_vertexBuffer = other.m_vertexBuffer;
	m_vertexBufferView = other.m_vertexBufferView;
	m_indexBuffer = other.m_indexBuffer;
	m_indexBufferView = other.m_indexBufferView;
	m_defaultHeapProperty = other.m_defaultHeapProperty;
	m_verticesBufferSizeDesc = other.m_verticesBufferSizeDesc;
	m_vBufferUploadHeap = other.m_vBufferUploadHeap;
	m_uploadHeapProperty = other.m_uploadHeapProperty;
	m_iBufferUploadHeap = other.m_iBufferUploadHeap;
	m_vertexData = other.m_vertexData;
	m_indexData = other.m_indexData;

	return *this;
}

bool Model::operator==(const Model& other) const {

	return (m_name == other.m_name);
}

bool Model::operator!=(const Model& other) const {

	return (m_name != other.m_name);
}