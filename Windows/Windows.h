#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include "../D3D12/Device/Device.h"
#include "../Mesh/MeshRenderer/MeshRenderer.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Window {


	public:
		Window(Device* device, LPCTSTR name, int width, int height, bool fullscreen, bool vSync, HINSTANCE hInstance, int showWnd);
		~Window();

		bool Update();
		void AddRenderedMesh(MeshRenderer* renderer);
		void RemoveRenderedMesh(MeshRenderer* renderer);

		int GetFrameIndex() const;
		int GetWindowWidth() const;
		int GetWindowHeight() const;
		int GetClientWidth() const;
		int GetClientHeight() const;
		int GetResolutionWidth() const;
		int GetResolutionHeight() const;
		bool GetHasFocus() const;
		bool GetIsMinimised() const;
		bool GetFullScreen() const;
		bool GetRunning() const;
		HWND GetHandle() const;
		const float* GetClearColor() const;
		std::vector<MeshRenderer*> GetRenderedMeshes() const;

		void CloseWindow();
		void SetClearColor(float R, float G, float B, float A);
		bool SetResolution(int width, int height);
		void SetWindowSize(int width, int height);
		void SetClientSize(int width, int height);
		void ToggleFullscreen();

		static std::vector<ResourcesToInit> m_resourcesToInit;
	private:

		Device* m_device;

		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain; // swapchain used to switch between render targets
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue; // container for command lists
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets
		Microsoft::WRL::ComPtr<ID3D12Resource>* m_renderTargets; // number of render targets equal to buffer count
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>* m_commandAllocator; // we want enough allocators for each buffer * number of threads (we only have one thread)
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList; // a command list we can record commands into, then execute them to render the frame
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;    // an object that is locked while our command list is being executed by the gpu. We need as many 
											 //as we have allocators (more if we want to know when the gpu is finished with an asset)
		Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer; // This is the memory for our depth buffer. it will also be used for a stencil buffer in a later tutorial
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsDescriptorHeap; // This is a heap for our depth/stencil buffer descriptor
		HANDLE m_fenceEvent; // a handle to an event when our fence is unlocked by the gpu
		UINT64* m_fenceValue; // this value is incremented each frame. each fence will have its own value
		int m_frameIndex; // current rtv we are on
		int m_rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)

		HWND m_hwnd = NULL;
		LPCTSTR m_windowName = L"WindowsName";
		LPCTSTR m_windowTitle = L"WindowsTitle";
		int m_windowWidth = 800;
		int m_windowHeight = 600;
		int m_clientRectWidth = 784;
		int m_clientRectHeight = 561;
		int m_resolutionWidth = 784;
		int m_resolutionHeight = 561;
		RECT m_oldScreenRect;
		unsigned int m_numerator;
		unsigned int m_denominator;
		bool m_hasFocus = true;
		bool m_minimised = false;
		bool m_fullscreen = false;
		bool m_vSync = false;
		bool m_running = true;
		float m_clearColor[4];
		CD3DX12_RESOURCE_BARRIER m_presentToTargetBarrier;
		CD3DX12_RESOURCE_BARRIER m_targetToPresentBarrier;
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissorRect;

		std::vector<MeshRenderer*> m_renderedMeshes;

		static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
		bool InitializeWindow(HINSTANCE hInstance, int showWnd, int width, int height, bool fullscreen);
		bool InitD3D12();
		void UpdatePipeline();
		void Render();
		void UpdateViewportAndScissorRectangle();
		bool OnSizeChanged(int width, int height);
		void WaitForGpu();
		void MoveToNextFrame();
		void UpdateTitle();
		void UpdateClientRect();
};

#endif