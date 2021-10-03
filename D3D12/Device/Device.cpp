#include "Device.h"

using namespace Microsoft::WRL;

Device::Device(int frameBufferSize) : m_frameBufferSize(frameBufferSize)
{
	//ID3D12Debug *pDebug = NULL;
 //   D3D12GetDebugInterface(__uuidof(ID3D12Debug),(void**)&pDebug);
 //   pDebug->EnableDebugLayer();
 //   pDebug->Release();

	if (!InitDevice())
		MessageBox(0, L"Device Initialization - Failed", L"Error", MB_OK);
}

Device::~Device()
{
	SAFE_DELETE_ARRAY(m_displayModeList);
}

bool Device::InitDevice()
{

	HRESULT hr;

	m_featureLevel = D3D_FEATURE_LEVEL_12_1;

	// Create the Direct3D 12 device.
	hr = D3D12CreateDevice(NULL, m_featureLevel, IID_PPV_ARGS(&m_device));
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create a DirectX 12.1 device.  The default video card does not support DirectX 12.1.", L"DirectX Device Failure", MB_OK);
		return false;
	}

	// Create a DirectX graphics interface factory.
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_factory));
	if (FAILED(hr))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	hr = m_factory->EnumAdapters(0, &m_adapter);
	if (FAILED(hr))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	hr = m_adapter->EnumOutputs(0, &m_adapterOutput);
	if (FAILED(hr))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	hr = m_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_numModes, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	m_displayModeList = new DXGI_MODE_DESC[m_numModes];
	if (!m_displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	hr = m_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_numModes, m_displayModeList);
	if (FAILED(hr))
	{
		return false;
	}

	// Get the adapter (video card) description.
	hr = m_adapter->GetDesc(&m_adapterDesc);
	if (FAILED(hr))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(m_adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	m_videoCardDescription = m_adapterDesc.Description;

	return true;
}

//void Device::Unleak() {
//
//	ANTI_LEAK(m_device);
//}


ComPtr<ID3D12Device> Device::GetDevice() const		{ return m_device; }
ComPtr<IDXGIFactory4> Device::GetFactory() const	{ return m_factory; }
ComPtr<IDXGIAdapter> Device::GetAdapter() const		{ return m_adapter; }
ComPtr<IDXGIOutput> Device::GetAdapterOutput() const{ return m_adapterOutput; }
DXGI_ADAPTER_DESC Device::GetAdapterDesc() const	{ return m_adapterDesc; }
D3D_FEATURE_LEVEL Device::GetFeatureLevel() const	{ return m_featureLevel; }
int Device::GetVideoCardMemory() const				{ return m_videoCardMemory; }
LPCWSTR Device::GetVideoCardDescription() const		{ return m_videoCardDescription; }
unsigned int Device::GetNumModes() const			{ return m_numModes; }
DXGI_MODE_DESC* Device::GetDisplayModeList() const	{ return m_displayModeList; }
const int Device::GetFrameBufferSize() const		{ return m_frameBufferSize; }