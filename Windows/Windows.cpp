#include "Windows.h"

using namespace Microsoft::WRL;

std::vector<ResourcesToInit> Window::m_resourcesToInit;

Window::Window(Device* device, LPCTSTR name, int width, int height, bool fullscreen, bool vSync, HINSTANCE hInstance, int showWnd) :
	m_device{ device },
	m_windowName{ name },
	m_windowTitle{ name },
	m_windowWidth{ width },
	m_windowHeight{ height },
	m_numerator{ 0 },
	m_denominator{ 1 },
	m_fullscreen{ fullscreen },
	m_vSync{ vSync },
	m_clearColor{ 0.0f, 0.2f, 0.4f, 1.0f },
	m_renderedMeshes{}
{
	m_renderTargets = new ComPtr<ID3D12Resource>[device->GetFrameBufferSize()]();
	m_commandAllocator = new ComPtr<ID3D12CommandAllocator>[device->GetFrameBufferSize()]();
	//m_fence = new ComPtr<ID3D12Fence>[device->GetFrameBufferSize()]();
	m_fenceValue = new UINT64[device->GetFrameBufferSize()]();

	if (!InitializeWindow(hInstance, showWnd, width, height, fullscreen))
		MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);


	if (!InitD3D12())
		MessageBox(0, L"D3D12 Initialization - Failed", L"Error", MB_OK);

	// Fill out the Viewport
	UpdateViewportAndScissorRectangle();
	UpdateTitle();
}

Window::~Window() {

	for (int i = 0; i < m_device->GetFrameBufferSize(); ++i)
	{
		m_frameIndex = i;
		WaitForGpu();
	}
	BOOL fs = false;
	if (m_swapChain->GetFullscreenState(&fs, NULL))
		m_swapChain->SetFullscreenState(false, NULL);

	SAFE_DELETE_ARRAY(m_renderTargets);
	SAFE_DELETE_ARRAY(m_commandAllocator);
	SAFE_DELETE_ARRAY(m_fenceValue);
	DestroyWindow(m_hwnd);

}

bool Window::Update() {

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			MessageBox(NULL, L"WM_QUIT", L"Error", MB_OK | MB_ICONERROR);
			m_running = false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
	{
		if(!m_minimised)
			Render();
	}
	return m_running;
}

void Window::AddRenderedMesh(MeshRenderer* renderer) {

	m_renderedMeshes.push_back(renderer);
}

void Window::RemoveRenderedMesh(MeshRenderer* renderer) {

	for (unsigned int i = 0; i < m_renderedMeshes.size(); ++i)
		if (m_renderedMeshes[i] == renderer)
			m_renderedMeshes.erase(m_renderedMeshes.begin() + i);
}


LRESULT CALLBACK Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* pParent = nullptr;

	// Get pointer to window
	if (msg == WM_CREATE)
	{
		pParent = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pParent);
	}
	else
	{
		pParent = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (!pParent) return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	pParent->m_hwnd = hwnd;
	return pParent->WndProc(msg, wParam, lParam);
}

LRESULT CALLBACK Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_CLOSE:
	{
		m_running = false;
		return 0;
	}

	case WM_SETFOCUS:
	{
		m_hasFocus = true;
		return 0;
	}

	case WM_KILLFOCUS:
	{
		m_hasFocus = false;
		return 0;
	}

	case WM_SIZE:
		if (m_running) {
			if (wParam == SIZE_MINIMIZED)
				m_minimised = true;
			else if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED)
				m_minimised = false;

			if (!m_minimised) {
				RECT windowRect = {};
				GetWindowRect(m_hwnd, &windowRect);
				int width = windowRect.right - windowRect.left;
				int height = windowRect.bottom - windowRect.top;
				if (width != m_windowWidth || height != m_windowHeight) {

					if (!OnSizeChanged(width, height)) {
						MessageBox(0, L"Failed to resize window",
							L"Error", MB_OK);
					}
				}
			}
		}
		return 0;
	}
	return DefWindowProc(m_hwnd, msg, wParam, lParam);
}

bool Window::InitializeWindow(HINSTANCE hInstance, int showWnd, int width, int height, bool fullscreen) {

	if (fullscreen)
	{
		HMONITOR hmon = MonitorFromWindow(m_hwnd,
			MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(hmon, &mi);

		width = mi.rcMonitor.right - mi.rcMonitor.left;
		height = mi.rcMonitor.bottom - mi.rcMonitor.top;
	}

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = StaticWndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	m_hwnd = CreateWindowEx(NULL,
		m_windowName,
		m_windowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		this);

	if (!m_hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (fullscreen)
	{
		SetWindowLong(m_hwnd, GWL_STYLE, 0);
	}

	ShowWindow(m_hwnd, showWnd);
	UpdateWindow(m_hwnd);

	return true;

}

bool Window::InitD3D12() {

	UpdateClientRect();

	HRESULT hr;

	// -- Create the Command Queue -- //

	D3D12_COMMAND_QUEUE_DESC cqDesc = {}; // we will be using all the default values

	hr = m_device->GetDevice()->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create command queue",
			L"Error", MB_OK);
		return false;
	}

	// -- Create the Swap Chain (double/tripple buffering) -- //

		// Now go through all the display modes and find the one that matches the screen height and width.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (unsigned int i = 0; i < m_device->GetNumModes(); i++)
	{
		if (m_device->GetDisplayModeList()[i].Height == static_cast<unsigned int>(m_clientRectHeight))
		{
			if (m_device->GetDisplayModeList()[i].Width == static_cast<unsigned int>(m_clientRectWidth))
			{
				m_numerator = m_device->GetDisplayModeList()[i].RefreshRate.Numerator;
				m_denominator = m_device->GetDisplayModeList()[i].RefreshRate.Denominator;
			}
		}
	}

	DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
	backBufferDesc.Width = m_clientRectWidth;
	backBufferDesc.Height = m_clientRectHeight;
	backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

	// describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = m_device->GetFrameBufferSize(); // number of buffers we have
	swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
	swapChainDesc.OutputWindow = m_hwnd; // handle to our window
	swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
	swapChainDesc.Windowed = !m_fullscreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

		// Set the refresh rate of the back buffer.
	if (m_vSync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	ComPtr<IDXGISwapChain> tempSwapChain;

	m_device->GetFactory()->CreateSwapChain(
		m_commandQueue.Get(), // the queue will be flushed once the swap chain is created
		&swapChainDesc, // give it the swap chain description we created above
		&tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
	);

	//Cast it so we can get current back buffer
	hr = tempSwapChain.As(&m_swapChain);
	if (FAILED(hr))
		MessageBox(0, L"Failed to cast SwapChain",
			L"Error", MB_OK);

	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// -- Create the Back Buffers (render target views) Descriptor Heap -- //

// describe an rtv descriptor heap and create
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = m_device->GetFrameBufferSize(); // number of descriptors for this heap.
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

	// This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
	// otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_device->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvDescriptorHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create rtv descriptor heap",
			L"Error", MB_OK);
		return false;
	}
	m_rtvDescriptorHeap->SetName(L"RTV descriptor heap");
	// get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
	// descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
	// device to give us the size. we will use this size to increment a descriptor handle offset
	m_rtvDescriptorSize = m_device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
	// but we cannot literally use it like a c++ pointer.
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
	for (int i = 0; i < m_device->GetFrameBufferSize(); i++)
	{
		// first we get the n'th buffer in the swap chain and store it in the n'th
		// position of our ID3D12Resource array
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to get swapchain buffer",
				L"Error", MB_OK);
			return false;
		}
		m_renderTargets[i]->SetName(L"Render target ");
		// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
		m_device->GetDevice()->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);

		// we increment the rtv handle by the rtv descriptor size we got above
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}

	// -- Create the Command Allocators -- //
	for (int i = 0; i < m_device->GetFrameBufferSize(); i++)
	{
		hr = m_device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i]));
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to create command allocator",
				L"Error", MB_OK);
			return false;
		}
	}

	// create the command list with the first allocator
	hr = m_device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[0].Get(), NULL, IID_PPV_ARGS(&m_commandList));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create command list",
			L"Error", MB_OK);
		return false;
	}

	m_commandList->Close();

	// -- Create a Fence & Fence Event -- //

	// create the fences
	hr = m_device->GetDevice()->CreateFence(m_fenceValue[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create fence",
			L"Error", MB_OK);
		return false;
	}
	m_fenceValue[m_frameIndex]++;

	// create a handle to a fence event
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		MessageBox(0, L"Failed to create fence event",
			L"Error", MB_OK);
		return false;
	}

	// create a depth stencil descriptor heap so we can get a pointer to the depth stencil buffer
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		hr = m_device->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsDescriptorHeap));
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create depth/stencil descriptor heap",
			L"Error", MB_OK);
		m_running = false;
	}
	m_dsDescriptorHeap->SetName(L"Depth/Stencil Resource Heap");

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES dsDefaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC dsDescriptor = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_clientRectWidth, m_clientRectHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	hr = m_device->GetDevice()->CreateCommittedResource(
		&dsDefaultHeap,
		D3D12_HEAP_FLAG_NONE,
		&dsDescriptor,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&m_depthStencilBuffer)
	);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create depth/stencil default heap",
			L"Error", MB_OK);
		m_running = false;
	}
	m_depthStencilBuffer->SetName(L"Depth/Stencil buffer default heap");

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	m_device->GetDevice()->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilDesc, m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	WaitForGpu();
	return true;
}

void Window::UpdatePipeline()
{
	HRESULT hr;

	// We have to wait for the gpu to finish with the command allocator before we reset it
	WaitForGpu();

	// we can only reset an allocator once the gpu is done with it
	// resetting an allocator frees the memory that the command list was stored in
	hr = m_commandAllocator[m_frameIndex]->Reset();
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to reset command allocator",
			L"Error", MB_OK);
		m_running = false;
	}

	hr = m_commandList->Reset(m_commandAllocator[m_frameIndex].Get(), NULL);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to reset command list",
			L"Error", MB_OK);
		m_running = false;
	}

	// here we start recording commands into the commandList (which all the commands will be stored in the commandAllocator)

	// transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
	m_presentToTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->ResourceBarrier(1, &m_presentToTargetBarrier);

	for (unsigned int i = 0; i < m_resourcesToInit.size(); ++i) {

		UpdateSubresources(m_commandList.Get(), m_resourcesToInit[i].m_destination,
			m_resourcesToInit[i].m_intermediate, m_resourcesToInit[i].m_intermediateOffset,
			m_resourcesToInit[i].m_firstSubResource, m_resourcesToInit[i].m_numSubResources,
			m_resourcesToInit[i].m_srcData);
		// transition the vertex buffer data from copy destination state to vertex buffer state
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_resourcesToInit[i].m_destination, m_resourcesToInit[i].m_before, m_resourcesToInit[i].m_after);
		m_commandList->ResourceBarrier(1, &barrier);

	}

	// here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	// get a handle to the depth/stencil buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
	m_commandList->ClearRenderTargetView(rtvHandle, m_clearColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (!m_renderedMeshes.empty()) {
		PSO* oldPso = nullptr;
		for (unsigned int i = 0; i < m_renderedMeshes.size(); ++i) {

			if (oldPso != nullptr) {
				if (*oldPso->GetRootSignature() != *m_renderedMeshes[i]->GetRootSignature())
					m_commandList->SetGraphicsRootSignature(m_renderedMeshes[i]->GetRootSignature()->GetRootSignature().Get());
				if (*oldPso != *m_renderedMeshes[i]->GetPso())
					m_commandList->SetPipelineState(m_renderedMeshes[i]->GetPso()->GetPso().Get());
			}
			else {
				m_commandList->SetGraphicsRootSignature(m_renderedMeshes[i]->GetRootSignature()->GetRootSignature().Get());
				m_commandList->SetPipelineState(m_renderedMeshes[i]->GetPso()->GetPso().Get());
			}

			for (unsigned int j = 0; j < m_renderedMeshes[i]->GetConstantBuffers().size(); ++j)
				m_commandList->SetGraphicsRootConstantBufferView(j, m_renderedMeshes[i]->GetConstantBuffers()[j]->GetCbUploadHeap()[m_frameIndex]->GetGPUVirtualAddress());

			if (m_renderedMeshes[i]->GetTextures().size()) {
				ID3D12DescriptorHeap* descriptorHeaps[] = { m_renderedMeshes[i]->GetMainDescriptorHeap().Get() };
				m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
				m_commandList->SetGraphicsRootDescriptorTable(m_renderedMeshes[i]->GetConstantBuffers().size(), m_renderedMeshes[i]->GetMainDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
			}
			m_commandList->IASetVertexBuffers(0, 1, m_renderedMeshes[i]->GetModel()->GetVertexBufferView());
			m_commandList->IASetIndexBuffer(m_renderedMeshes[i]->GetModel()->GetIndexBufferView());
			m_commandList->DrawIndexedInstanced(m_renderedMeshes[i]->GetModel()->GetIndicesSize(), 1, 0, 0, 0);

			oldPso = m_renderedMeshes[i]->GetPso();
		}
	}

	// transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
	// warning if present is called on the render target when it's not in the present state
	m_targetToPresentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &m_targetToPresentBarrier);

	hr = m_commandList->Close();
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to close command list",
			L"Error", MB_OK);
		m_running = false;
	}
}

void Window::Render()
{
	HRESULT hr;

	UpdatePipeline(); // update the pipeline by sending commands to the commandqueue

	// create an array of command lists (only one command list here)
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };

	// execute the array of command lists
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// present the current backbuffer
	hr = m_swapChain->Present(0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to swapchain present",
			L"Error", MB_OK);
		m_running = false;
	}

	MoveToNextFrame();

	if (!m_resourcesToInit.empty())
		m_resourcesToInit.clear();
}

void Window::UpdateViewportAndScissorRectangle() {

	float viewWidthRatio = static_cast<float>(m_resolutionWidth) / m_clientRectWidth;
	float viewHeightRatio = static_cast<float>(m_resolutionHeight) / m_clientRectHeight;

	float x = 1.0f;
	float y = 1.0f;

	if (viewWidthRatio < viewHeightRatio)
	{
		// The scaled image's height will fit to the viewport's height and 
		// its width will be smaller than the viewport's width.
		x = viewWidthRatio / viewHeightRatio;
	}
	else
	{
		// The scaled image's width will fit to the viewport's width and 
		// its height may be smaller than the viewport's height.
		y = viewHeightRatio / viewWidthRatio;
	}

	m_viewport.TopLeftX = m_clientRectWidth * (1.0f - x) * 0.5f;
	m_viewport.TopLeftY = m_clientRectHeight * (1.0f - y) * 0.5f;
	m_viewport.Width = x * m_clientRectWidth;
	m_viewport.Height = y * m_clientRectHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_scissorRect.left = static_cast<LONG>(m_viewport.TopLeftX);
	m_scissorRect.right = static_cast<LONG>(m_viewport.TopLeftX + m_viewport.Width);
	m_scissorRect.top = static_cast<LONG>(m_viewport.TopLeftY);
	m_scissorRect.bottom = static_cast<LONG>(m_viewport.TopLeftY + m_viewport.Height);
}


bool Window::SetResolution(int width, int height) {

	SetClientSize(width, height);

	RECT windowRect = {};
	GetClientRect(m_hwnd, &windowRect);
	int clientWidth = windowRect.right - windowRect.left;
	int clientHeight = windowRect.bottom - windowRect.top;

	m_resolutionWidth = clientWidth;
	m_resolutionHeight = clientHeight;

	UpdateViewportAndScissorRectangle();
	UpdateTitle();

	return true;
}

void Window::SetWindowSize(int width, int height) {
	if (!m_fullscreen)
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

void Window::SetClientSize(int width, int height)
{
	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = width;
	windowRect.top = 0;
	windowRect.bottom = height;
	AdjustWindowRect(&windowRect, GetWindowLong(m_hwnd, GWL_STYLE), FALSE);
	SetWindowPos(m_hwnd, 0,
		windowRect.left, windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

	//UpdateWindow(m_hwnd);
}

void Window::ToggleFullscreen() {
	if (m_fullscreen)
	{
		// Restore the window's attributes and size.
		SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

		SetWindowPos(
			m_hwnd,
			HWND_NOTOPMOST,
			m_oldScreenRect.left,
			m_oldScreenRect.top,
			m_oldScreenRect.right - m_oldScreenRect.left,
			m_oldScreenRect.bottom - m_oldScreenRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(m_hwnd, SW_NORMAL);
	}
	else
	{
		HRESULT hr;
		// Save the old window rect so we can restore it when exiting fullscreen mode.
		GetWindowRect(m_hwnd, &m_oldScreenRect);

		// Make the window borderless so that the client area can fill the screen.
		SetWindowLong(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

		RECT fullscreenWindowRect;
		if (m_swapChain)
		{
			// Get the settings of the display on which the app's window is currently displayed
			ComPtr<IDXGIOutput> pOutput;
			hr = m_swapChain->GetContainingOutput(&pOutput);
			if (FAILED(hr))
				MessageBox(0, L"Failed to GetContainingOuput from swapchain",
					L"Error", MB_OK);
			DXGI_OUTPUT_DESC Desc;
			hr = pOutput->GetDesc(&Desc);
			if (FAILED(hr))
				MessageBox(0, L"Failed to get swapchain desc",
					L"Error", MB_OK);
			fullscreenWindowRect = Desc.DesktopCoordinates;
		}
		else
		{
			// Get the settings of the primary display
			DEVMODE devMode = {};
			devMode.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

			fullscreenWindowRect = {
				devMode.dmPosition.x,
				devMode.dmPosition.y,
				devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
				devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)
			};
		}

		SetWindowPos(
			m_hwnd,
			HWND_TOP,
			fullscreenWindowRect.left,
			fullscreenWindowRect.top,
			fullscreenWindowRect.right,
			fullscreenWindowRect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);


		ShowWindow(m_hwnd, SW_MAXIMIZE);
	}

	m_fullscreen = !m_fullscreen;
}

bool Window::OnSizeChanged(int width, int height) {
	// Determine if the swap buffers and other resources need to be resized or not.
	// Flush all current GPU commands.
	HRESULT hr;
	WaitForGpu();

	// Release the resources holding references to the swap chain (requirement of
	// IDXGISwapChain::ResizeBuffers) and reset the frame fence values to the
	// current fence value.
	for (UINT i = 0; i < m_device->GetFrameBufferSize(); i++)
	{
		m_renderTargets[i].Reset();
		m_fenceValue[i] = m_fenceValue[m_frameIndex];
	}

	hr = m_depthStencilBuffer.Reset();
	if (FAILED(hr)) {
		MessageBox(0, L"Failed to reset DepthStencil descriptor heap",
			L"Error", MB_OK);
		return false;
	}

	UpdateClientRect();
	unsigned int tempNumerator = 0.0f;
	unsigned int tempDenominator = 1.0f;
	for (unsigned int i = 0; i < m_device->GetNumModes(); i++) {
		if (m_device->GetDisplayModeList()[i].Height == static_cast<unsigned int>(m_clientRectHeight)) {
			if (m_device->GetDisplayModeList()[i].Width == static_cast<unsigned int>(m_clientRectWidth)) {

				tempNumerator = m_device->GetDisplayModeList()[i].RefreshRate.Numerator;
				tempDenominator = m_device->GetDisplayModeList()[i].RefreshRate.Denominator;
			}
		}
	}

	// Resize the swap chain to the desired dimensions.
	DXGI_SWAP_CHAIN_DESC desc = {};
	m_swapChain->GetDesc(&desc);
	// Set the refresh rate of the back buffer.
	if (m_vSync)
	{
		desc.BufferDesc.RefreshRate.Numerator = tempNumerator;
		desc.BufferDesc.RefreshRate.Denominator = tempDenominator;
	}
	else
	{
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	hr = m_swapChain->ResizeBuffers(m_device->GetFrameBufferSize(), m_clientRectWidth, m_clientRectHeight, desc.BufferDesc.Format, desc.Flags);
	if (FAILED(hr)) {
		MessageBox(0, L"Failed to resize swapChain buffers",
			L"Error", MB_OK);
		return false;
	}

	// Reset the frame index to the current back buffer index.
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	m_windowWidth = width;
	m_windowHeight = height;
	m_numerator = tempNumerator;
	m_denominator = tempDenominator;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < m_device->GetFrameBufferSize(); i++)
	{
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		if (FAILED(hr)) {
			MessageBox(0, L"Failed to GetBuffer from swapChain",
				L"Error", MB_OK);
			return false;
		}
		m_device->GetDevice()->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES dsDefaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC dsDescriptor = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_clientRectWidth, m_clientRectHeight, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	m_device->GetDevice()->CreateCommittedResource(
		&dsDefaultHeap,
		D3D12_HEAP_FLAG_NONE,
		&dsDescriptor,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&m_depthStencilBuffer)
	);

	m_device->GetDevice()->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilDesc, m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	m_resolutionWidth = m_clientRectWidth;
	m_resolutionHeight = m_clientRectHeight;

	UpdateViewportAndScissorRectangle();
	UpdateTitle();
}

// Wait for pending GPU work to complete.
void Window::WaitForGpu()
{
	HRESULT hr;
	// Schedule a Signal command in the queue.
	hr = m_commandQueue->Signal(m_fence.Get(), m_fenceValue[m_frameIndex]);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to signal",
			L"Error", MB_OK);
	}

	// Wait until the fence has been processed.
	hr = m_fence->SetEventOnCompletion(m_fenceValue[m_frameIndex], m_fenceEvent);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to set event on completion",
			L"Error", MB_OK);
	}
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	m_fenceValue[m_frameIndex]++;
}

// Prepare to render the next frame.
void Window::MoveToNextFrame()
{
	HRESULT hr;

	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = m_fenceValue[m_frameIndex];
	hr = m_commandQueue->Signal(m_fence.Get(), currentFenceValue);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to signal",
			L"Error", MB_OK);
	}
	// Update the frame index.
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (m_fence->GetCompletedValue() < m_fenceValue[m_frameIndex])
	{
		hr = m_fence->SetEventOnCompletion(m_fenceValue[m_frameIndex], m_fenceEvent);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to set event on completion",
				L"Error", MB_OK);
		}
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	m_fenceValue[m_frameIndex] = currentFenceValue + 1;
}

void Window::UpdateTitle()
{
	RECT windowRect = {};
	GetWindowRect(m_hwnd, &windowRect);
	RECT clientRect = {};
	GetClientRect(m_hwnd, &clientRect);
	// Update resolutions shown in app title.
	wchar_t updatedTitle[256];
	swprintf_s(updatedTitle, L"Client Size: ( %u x %u ), Window size: ( %u x %u ), Resolution: ( %u x %u )", m_clientRectWidth, m_clientRectHeight, m_windowWidth, m_windowHeight, m_resolutionWidth, m_resolutionHeight);
	std::wstring windowText = updatedTitle;
	SetWindowText(m_hwnd, windowText.c_str());
}

void Window::UpdateClientRect() {

	RECT clientRect = {};
	GetClientRect(m_hwnd, &clientRect);
	m_clientRectWidth = clientRect.right - clientRect.left;
	m_clientRectHeight = clientRect.bottom - clientRect.top;
}

int Window::GetFrameIndex() const								{ return m_frameIndex; }
int Window::GetWindowWidth() const								{ return m_windowWidth; }
int Window::GetWindowHeight() const								{ return m_windowHeight; }
int Window::GetClientWidth() const								{ return m_clientRectWidth; }
int Window::GetClientHeight() const								{ return m_clientRectHeight; }
int Window::GetResolutionWidth() const							{ return m_resolutionWidth; }
int Window::GetResolutionHeight() const							{ return m_resolutionHeight; }
bool Window::GetHasFocus() const								{ return m_hasFocus; }
bool Window::GetIsMinimised() const								{ return m_minimised; }
bool Window::GetFullScreen() const								{ return m_fullscreen; }
bool Window::GetRunning() const									{ return m_running; }
HWND Window::GetHandle() const									{ return m_hwnd; }
const float* Window::GetClearColor() const						{ return m_clearColor; }
std::vector<MeshRenderer*> Window::GetRenderedMeshes() const	{ return m_renderedMeshes; }

void Window::CloseWindow() {

	m_running = false;
}

void Window::SetClearColor(float R, float G, float B, float A)
{
	m_clearColor[0] = R;
	m_clearColor[1] = G;
	m_clearColor[2] = B;
	m_clearColor[3] = A;
}