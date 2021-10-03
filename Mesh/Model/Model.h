#ifndef _MODEL_H_
#define _MODEL_H_

#include "../Vertex.h"
#include "../../D3D12/Device/Device.h"
#include <vector>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Model{

	public:
		Model(Device* device, LPCWSTR name, std::vector<Vertex> vertices, std::vector<int> indices);
		Model(Device* device, const char* name, std::vector<Vertex> vertices, std::vector<int> indices);
		Model(const Model& other);
		Model(Model&& other) noexcept;
		~Model();

		const wchar_t* GetPathPtr() const;

		std::vector<Vertex> GetVertices() const;
		std::vector<int> GetIndices() const;

		unsigned int GetVerticesSize() const;
		unsigned int GetVerticesSizeInBytes() const;
		unsigned int GetIndicesSize() const;
		unsigned int GetIndicesSizeInBytes() const;

		Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer() const;
		D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() const;
		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer() const;
		D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const;

		Model& operator=(const Model& other);
		Model& operator=(Model&& other) noexcept;
		bool operator==(const Model& other) const;
		bool operator!=(const Model& other) const;

	private:
		std::vector<wchar_t> m_name;

		std::vector<Vertex> m_vertices;
		std::vector<int> m_indices;

		unsigned int m_verticesSize;
		unsigned int m_verticesSizeInBytes;
		unsigned int m_indicesSize;
		unsigned int m_indicesSizeInBytes;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer; // a default buffer in GPU memory that we will load vertex data for our triangle into
		D3D12_VERTEX_BUFFER_VIEW* m_vertexBufferView; // a structure containing a pointer to the vertex data in gpu memory
												   // the total size of the buffer, and the size of each element (vertex)
		Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer; // a default buffer in GPU memory that we will load index data for our triangle into
		D3D12_INDEX_BUFFER_VIEW* m_indexBufferView; // a structure holding information about the index buffer

		CD3DX12_HEAP_PROPERTIES m_defaultHeapProperty;
		CD3DX12_RESOURCE_DESC m_verticesBufferSizeDesc;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_vBufferUploadHeap;
		CD3DX12_HEAP_PROPERTIES m_uploadHeapProperty;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_iBufferUploadHeap;
		D3D12_SUBRESOURCE_DATA m_vertexData;
		D3D12_SUBRESOURCE_DATA m_indexData;

};
#endif // !_MODEL_H_

