#ifndef _RHI_H_
#define _RHI_H_

#include "../D3D12/ResourceHandler/Dx12ResourceHandler.h"
#include "../Windows/Windows.h"
#include "../Loaders/FBXLoader.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Rhi {

public:

	Rhi(const int frameBufferSize);
	~Rhi();

	void GenWindow(LPCTSTR name, int width, int height, bool fullscreen, bool vSync, HINSTANCE hInstance, int showWnd);
	Shader* GetShader(LPCWSTR path, D3D12_SHADER_VISIBILITY visibility);
	Texture* GetTexture(LPCWSTR path); 
	MeshRenderer* GetMeshRenderer(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, Model* model, std::vector<Shader*>& shaders,
									std::vector<Texture*>& textures, bool hasStaticSampler, std::vector<ConstantBuffer*>& cbs);
	std::vector<Model*> LoadFBX(LPCWSTR path);
	Model* LoadObj(LPCWSTR path);

	template<typename T>
	ConstantBuffer* GetConstantBuffer(T* data, int sizeofData, int registerNb, D3D12_SHADER_VISIBILITY shaderVisibility) {


	}

	//TODO
	template<typename T>
	void UpdateConstantBuffer(ConstantBuffer* cb, T* dataPtr, int dataSize) {
		for (size_t i{ 0 }; i < m_device->GetFrameBufferSize(); ++i)
			cb->Update(dataPtr, dataSize, i);
	}

	Model* GetModel(LPCWSTR path);
	bool Update();

	Device* GetDevice() const;
	std::vector<Window*> GetWindows() const;
	std::vector<Window*>& GetWindows();
	Window* GetWindow(unsigned int index) const;
	std::vector<Model*> GetModels()const;
	const int GetFrameBufferSize() const;

	void SetActive(MeshRenderer* renderer, Window* window, bool active);

private:

	Device* m_device;
	std::vector<Window*> m_windows;
	std::vector<Model*> m_models;
	std::vector<ConstantBuffer*> m_cbs;
	Dx12ResourceHandler* m_resourceHandler;
	FBXLoader* m_fbxLoader;
	const int m_frameBufferSize;
	bool m_hasActiveWindow;
};

#endif
