#include "RHI.h"
#include "../Loaders/OBJLoader.h"
#include "../Loaders/BasicObjectLoader.h"

Rhi::Rhi(int frameBufferSize) :
	m_frameBufferSize{ frameBufferSize },
	m_device{ new Device(frameBufferSize) },
	m_hasActiveWindow{ false },
	m_resourceHandler{ new Dx12ResourceHandler(m_device) },
	m_fbxLoader{ new FBXLoader(m_device) }
{
	m_models.push_back(LoadCube(m_device));
}

Rhi::~Rhi() {
	SAFE_DELETE_VECTOR(m_models);
	SAFE_DELETE_VECTOR(m_cbs);
	SAFE_DELETE_VECTOR(m_windows);
	SAFE_DELETE(m_resourceHandler);
	SAFE_DELETE(m_fbxLoader);
	SAFE_DELETE(m_device);
}

void Rhi::GenWindow(LPCTSTR name, int width, int height, bool fullscreen, bool vSync, HINSTANCE hInstance, int showWnd) {

	Window* wnd = new Window(m_device, name, width, height, fullscreen, vSync, hInstance, showWnd);
	m_windows.push_back(wnd);
	m_hasActiveWindow = true;
}

Shader* Rhi::GetShader(LPCWSTR path, D3D12_SHADER_VISIBILITY visibility) {
	return m_resourceHandler->GetShader(path, visibility);
}

Texture* Rhi::GetTexture(LPCWSTR path) {
	return m_resourceHandler->GetTexture(path);
}

MeshRenderer* Rhi::GetMeshRenderer(D3D12_INPUT_LAYOUT_DESC* inputLayoutDesc, Model* model, std::vector<Shader*>& shaders,
									std::vector<Texture*>& textures, bool hasStaticSampler, std::vector<ConstantBuffer*>& cbs) {
	return m_resourceHandler->GetMeshRenderer(inputLayoutDesc, model, shaders, textures, hasStaticSampler, cbs);
}

Model* Rhi::GetModel(LPCWSTR name) {

	for (unsigned int i = 0; i < m_models.size(); ++i)
		if (wcscmp(m_models[i]->GetPathPtr(), name) == 0)
			return m_models[i];

	return nullptr;
}

Model* Rhi::LoadObj(LPCWSTR path) {

	m_models.push_back(ExtractModel(m_device, path));
	return m_models.back();
}

std::vector<Model*> Rhi::LoadFBX(LPCWSTR path) {

	std::vector<Model*> temp{ m_fbxLoader->ExtractFile(path) };
	m_models.insert(m_models.end(), temp.begin(), temp.end());

	return temp;
}


bool Rhi::Update() {

	if (m_windows.size() > 0) {
		for (unsigned int i = 0; i < m_windows.size(); ++i) {
			if (!m_windows[i]->Update()) {
				SAFE_DELETE(m_windows[i]);
				m_windows.erase(m_windows.begin() + i);
			}
		}
		return true;
	}
	return false;
}

Device* Rhi::GetDevice() const { return m_device; }
std::vector<Window*> Rhi::GetWindows() const { return m_windows; }
std::vector<Window*>& Rhi::GetWindows() { return m_windows; }
Window* Rhi::GetWindow(unsigned int index) const
{
	return m_windows[index];
}
std::vector<Model*> Rhi::GetModels()const { return m_models; }

const int Rhi::GetFrameBufferSize() const { return m_device->GetFrameBufferSize(); }

void Rhi::SetActive(MeshRenderer* renderer, Window* window, bool active) {

	if (active) {
		for (unsigned int i = 0; i < window->GetRenderedMeshes().size(); ++i)
			if (window->GetRenderedMeshes()[i] == renderer)
				return;
		window->AddRenderedMesh(renderer);
	}
	else {

		window->RemoveRenderedMesh(renderer);
	}

}
