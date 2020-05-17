#pragma once

#include "../Core/Core.h"
#include "../Core/Rect2D.h"
#include "D3D11Api.h"
#include "../Framework/NativeWindow.h"
#include "D3D11Viewport.h"

namespace Bs::Rendering
{
	struct D3D11ViewportSettings
	{
		Rect2D geometry;
	};

	struct D3D11BackendSettings
	{
		D3D11ViewportSettings viewport;
	};

	class D3D11Backend
	{
	public:
		using Settings = D3D11BackendSettings;

	public:
		void Initialize(const Settings& settings);
		void Shutdown();
		void Draw();

		void CreateViewport(D3D11ViewportSettings viewportSettings, HWND windowHandle);

	protected:
		u32 m_msaaSamples = 4;
		u32 m_msaaQualityLevels;
		D3D_FEATURE_LEVEL m_featureLevel;
		DXGI_FORMAT m_swapChainFormat;
		WinOs::ComPtr<ID3D11Device5>		m_device;
		WinOs::ComPtr<ID3D11DeviceContext4> m_deviceContext;
		UniquePtr<D3D11Viewport> m_viewport;

		WinOs::ComPtr<IDXGISwapChain1> m_swapChain;
		WinOs::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		WinOs::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	};
}
