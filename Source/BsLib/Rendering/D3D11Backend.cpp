#include "D3D11Backend.h"
#include "../Core/Assertion.h"
#include "../Core/FixedArray.h"

#define VERIFYD3D11RESULT(x)			{HRESULT hr = x; if (FAILED(hr)) { PANIC(); }}

namespace Bs::Rendering
{
	void D3D11Backend::Initialize(const Settings& settings)
	{
		m_swapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		u32 deviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		FixedArray<D3D_FEATURE_LEVEL, 4> featureLevels = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		// Create ID3D11Device and ID3D11DeviceContext.
		WinOs::ComPtr<ID3D11Device> direct3DDevice;
		WinOs::ComPtr<ID3D11DeviceContext> direct3DDeviceContext;
		VERIFYD3D11RESULT(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			deviceFlags,
			featureLevels.Data(),
			featureLevels.Size(),
			D3D11_SDK_VERSION,
			direct3DDevice.GetAddressOf(),
			&m_featureLevel,
			direct3DDeviceContext.GetAddressOf()));

		BS_ASSERT(direct3DDevice != nullptr);
		BS_ASSERT(direct3DDeviceContext != nullptr);

		// Get the ID3D11Device5 interface.
		VERIFYD3D11RESULT(direct3DDevice->QueryInterface(__uuidof(ID3D11Device5), (void**)m_device.GetAddressOf()));
		BS_ASSERT(m_device != nullptr);

		// Get the ID3D11DeviceContext4 interface.
		direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)m_deviceContext.GetAddressOf());
		BS_ASSERT(m_deviceContext != nullptr);

		// Check device support for MSAA.
		VERIFYD3D11RESULT(m_device->CheckMultisampleQualityLevels(m_swapChainFormat, m_msaaSamples, &m_msaaQualityLevels));
		BS_ASSERT_FMT(m_msaaQualityLevels > 0, "Unsupported multi-sampling quality");
	}

	void D3D11Backend::Shutdown()
	{
		m_deviceContext->ClearState();
		m_deviceContext->Flush();
	}

	void D3D11Backend::Draw()
	{
		const float backgroundColor[4] { 0.392f, 0.584f, 0.929f, 1.0f };		
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), backgroundColor);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		VERIFYD3D11RESULT(m_swapChain->Present(0, 0));
	}

	void D3D11Backend::CreateViewport(D3D11ViewportSettings viewportSettings, HWND windowHandle)
	{
		// Create the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ 0 };
		swapChainDesc.Width = viewportSettings.geometry.width;
		swapChainDesc.Height = viewportSettings.geometry.height;
		swapChainDesc.Format = m_swapChainFormat;
		swapChainDesc.SampleDesc.Count = m_msaaSamples;
		swapChainDesc.SampleDesc.Quality = m_msaaQualityLevels - 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{ 0 };
		fullScreenDesc.RefreshRate.Numerator = 60;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = true;

		WinOs::ComPtr<IDXGIDevice4> dxgiDevice;
		VERIFYD3D11RESULT(m_device->QueryInterface(__uuidof(IDXGIDevice4), (void**)dxgiDevice.GetAddressOf()));
		BS_ASSERT(dxgiDevice != nullptr);

		WinOs::ComPtr<IDXGIAdapter> dxgiAdapter;
		VERIFYD3D11RESULT(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
		BS_ASSERT(dxgiAdapter != nullptr);

		WinOs::ComPtr<IDXGIFactory2> dxgiFactory;
		VERIFYD3D11RESULT(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));
		BS_ASSERT(dxgiFactory != nullptr);

		VERIFYD3D11RESULT(dxgiFactory->CreateSwapChainForHwnd(
			m_device.Get(),
			windowHandle,
			&swapChainDesc,
			nullptr /*Windowed swap chain*/,
			nullptr,
			m_swapChain.GetAddressOf()));

		// Create the render target view.
		WinOs::ComPtr<ID3D11Texture2D> backBuffer;
		VERIFYD3D11RESULT(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));
		VERIFYD3D11RESULT(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf()));

		// Create the depth stencil buffer.
		D3D11_TEXTURE2D_DESC depthStencilDesc{ 0 };
		depthStencilDesc.Width = viewportSettings.geometry.width;
		depthStencilDesc.Height = viewportSettings.geometry.height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.SampleDesc.Count = m_msaaSamples;
		depthStencilDesc.SampleDesc.Quality = m_msaaQualityLevels - 1;

		WinOs::ComPtr<ID3D11Texture2D> depthStencilBuffer;
		VERIFYD3D11RESULT(m_device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.GetAddressOf()));
		VERIFYD3D11RESULT(m_device->CreateDepthStencilView(
			depthStencilBuffer.Get(), 
			nullptr, 
			m_depthStencilView.GetAddressOf()));

		// Bind the render target.
		auto renderTargetViews = m_renderTargetView.Get();
		m_deviceContext->OMSetRenderTargets(1, &renderTargetViews, m_depthStencilView.Get());

		// Set the viewport.
		D3D11_VIEWPORT viewport = CD3D11_VIEWPORT(
			static_cast<float>(viewportSettings.geometry.x),
			static_cast<float>(viewportSettings.geometry.y),
			static_cast<float>(viewportSettings.geometry.width), 
			static_cast<float>(viewportSettings.geometry.width),
			0.0f, 1.0f);
		m_deviceContext->RSSetViewports(1, &viewport);
	}
}