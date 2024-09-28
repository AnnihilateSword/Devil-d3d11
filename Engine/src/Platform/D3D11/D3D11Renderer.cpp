#include "EnginePCH.h"

#include "D3D11Renderer.h"

using namespace DirectX;

namespace Devil
{
	D3D11Renderer::D3D11Renderer(HWND hwnd)
		:
		m_Hwnd{ hwnd },
		/** D3D11 Resources */
		m_Device{ nullptr },
		m_DeviceContext{ nullptr },
		m_SwapChain{ nullptr },
		m_RenderTargetView{ nullptr },
		m_DepthStencilBuffer{ nullptr },
		m_DepthStencilView{ nullptr },
		m_DepthStencilState{ nullptr },
		//m_DepthDisabledStencilState{ nullptr },
		//m_RasterizerState{ nullptr },
		m_ScreenViewport{}
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = hwnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		UINT swapCreateFlags{};
#ifdef _DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_DeviceContext
		);

		ComPtr<ID3D11Resource> pBackBuffer;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
		m_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_RenderTargetView);

		// create depth stensil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		ComPtr<ID3D11DepthStencilState> pDSState;
		m_Device->CreateDepthStencilState(&dsDesc, &m_DepthStencilState);

		// bind depth state
		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1u);

		// create depth stensil texture
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = 800u;
		descDepth.Height = 600u;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencilBuffer);

		// create view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &descDSV, &m_DepthStencilView);

		// bind depth stensil view to OM
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		// configure viewport
		m_ScreenViewport.Width = 800.0f;
		m_ScreenViewport.Height = 450.0f;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		m_ScreenViewport.TopLeftX = 0.0f;
		m_ScreenViewport.TopLeftY = 0.0f;
		m_DeviceContext->RSSetViewports(1u, &m_ScreenViewport);
	}

	void D3D11Renderer::EndFrame()
	{
		m_SwapChain->Present(1, 0);
	}

	void D3D11Renderer::ClearBuffer(float red, float green, float blue) noexcept
	{
		float color[4]{ red, green, blue, 1.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void D3D11Renderer::DrawIndexed(unsigned int count) noexcept
	{
		m_DeviceContext->DrawIndexed(count, 0u, 0u);
	}

	void D3D11Renderer::SetProjection(FXMMATRIX proj) noexcept
	{
		m_Projection = proj;
	}

	FXMMATRIX D3D11Renderer::GetProjection() const noexcept
	{
		return m_Projection;
	}
}