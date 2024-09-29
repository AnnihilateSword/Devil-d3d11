#include "EnginePCH.h"

#include "D3D11Renderer.h"

#include <Vendor/ImGui/imgui.h>
#include <Vendor/ImGui/imgui_impl_win32.h>
#include <Vendor/ImGui/imgui_impl_dx11.h>

using namespace DirectX;

namespace Devil
{
	D3D11Renderer::D3D11Renderer(HWND hwnd, unsigned int clientWidth, unsigned int clientHeight)
		:
		m_Hwnd{ hwnd },
		m_ClientWidth{ clientWidth },
		m_ClientHeight{ clientHeight },
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
		UINT CreateDeviceFlags{};
#ifdef _DEBUG
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};

		/** Create Device and Context */
		D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceFlags, featureLevels, 1u, D3D11_SDK_VERSION,
			&m_Device, nullptr, &m_DeviceContext);

		// Test the quality levels supported by MSAA
		m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4u, &m_4xMsaaQuality);
		assert(m_4xMsaaQuality > 0);


		DXGI_SWAP_CHAIN_DESC sd = {};
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = m_ClientWidth;
		sd.BufferDesc.Height = m_ClientHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		if (m_bEnable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_Hwnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;

		m_Device.As(&dxgiDevice);
		dxgiDevice->GetAdapter(&dxgiAdapter);
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf()));

		dxgiFactory1->CreateSwapChain(m_Device.Get(), &sd, &m_SwapChain);

		OnResize();


		/** ImGui Layer */
		// Setup Renderer backends
		ImGui_ImplDX11_Init(m_Device.Get(), m_DeviceContext.Get());
	}

	void D3D11Renderer::BeginFrame(float red, float green, float blue)
	{
		/** ImGui Layer */
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		float color[4]{ red, green, blue, 1.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);


		// ************************************************************************************************************
		// We must reset the RenderTarget at this location, otherwise when we drag the docking imgui out of the window, 
		// the d3d render content will not render to the desired rendertargetview ( imgui-docking note )
		// ************************************************************************************************************
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	}

	void D3D11Renderer::EndFrame()
	{
		/** ImGui Layer */
		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		/** Present */
		m_SwapChain->Present(1, 0);
	}

	void D3D11Renderer::DrawIndexed(unsigned int count) noexcept
	{
		m_DeviceContext->DrawIndexed(count, 0u, 0u);
	}

	void D3D11Renderer::OnResize()
	{
		// Release the resources associated with the rendering pipeline output
		m_RenderTargetView.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilBuffer.Reset();

		ComPtr<ID3D11Texture2D> pBackBuffer;
		m_SwapChain->ResizeBuffers(1u, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0u);
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
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
		descDepth.Width = m_ClientWidth;
		descDepth.Height = m_ClientHeight;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		if (m_bEnable4xMsaa)
		{
			descDepth.SampleDesc.Count = 4;
			descDepth.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
		}

		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencilBuffer);

		// create view of depth stensil texture
		m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, &m_DepthStencilView);

		// bind depth stensil view to OM
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		// configure viewport
		m_ScreenViewport.Width = m_ClientWidth;
		m_ScreenViewport.Height = m_ClientHeight;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		m_ScreenViewport.TopLeftX = 0.0f;
		m_ScreenViewport.TopLeftY = 0.0f;
		m_DeviceContext->RSSetViewports(1u, &m_ScreenViewport);
	}

	void D3D11Renderer::SetProjection(XMMATRIX proj) noexcept
	{
		m_Projection = proj;
	}

	void D3D11Renderer::SetCameraMatrix(DirectX::XMMATRIX camera) noexcept
	{
		m_CameraMatrix = camera;
	}

	DirectX::XMMATRIX D3D11Renderer::GetProjection() const noexcept
	{
		return m_Projection;
	}

	DirectX::XMMATRIX D3D11Renderer::GetCameraMatrix() const noexcept
	{
		return m_CameraMatrix;
	}
}