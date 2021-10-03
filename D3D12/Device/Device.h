#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "../d3dx12.h"
#include <dxgi1_4.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Device
{
	public:

		Device(const int frameBufferSize);
		~Device();

//		void Unleak();

		Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const;
		Microsoft::WRL::ComPtr<IDXGIFactory4> GetFactory() const;
		Microsoft::WRL::ComPtr<IDXGIAdapter> GetAdapter() const;
		Microsoft::WRL::ComPtr<IDXGIOutput> GetAdapterOutput() const;
		DXGI_ADAPTER_DESC GetAdapterDesc() const;
		D3D_FEATURE_LEVEL GetFeatureLevel() const;
		int GetVideoCardMemory() const;
		LPCWSTR GetVideoCardDescription() const;
		unsigned int GetNumModes() const;
		DXGI_MODE_DESC* GetDisplayModeList() const;
		const int GetFrameBufferSize() const;

	private:

		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		Microsoft::WRL::ComPtr<IDXGIFactory4> m_factory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_adapter;
		Microsoft::WRL::ComPtr<IDXGIOutput> m_adapterOutput;
		DXGI_ADAPTER_DESC m_adapterDesc;
		D3D_FEATURE_LEVEL m_featureLevel;
		int m_videoCardMemory;
		LPCWSTR m_videoCardDescription;
		unsigned int m_numModes;
		DXGI_MODE_DESC* m_displayModeList;
		const int m_frameBufferSize;

		bool InitDevice();

};
#endif