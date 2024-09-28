#pragma once

#include "EnginePCH.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl/client.h>

#include "Core/DevilException.h"


namespace Devil
{
	class D3D11Renderer
	{
		friend class D3D11Bindable;
	public:
		D3D11Renderer(HWND hwnd);
		virtual ~D3D11Renderer() = default;

	public:
		void EndFrame();
		void ClearBuffer(float red, float green, float blue) noexcept;
		void DrawIndexed(unsigned int count) noexcept;
		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DirectX::FXMMATRIX GetProjection() const noexcept;

	private:
		HWND m_Hwnd{};

		/** D3D11 Resources */
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D11Device> m_Device{};
		ComPtr<ID3D11DeviceContext> m_DeviceContext{};
		ComPtr<IDXGISwapChain> m_SwapChain{};

		// back buffer view in swap chain.
		ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};

		// depth stencil resources.
		ComPtr<ID3D11Texture2D> m_DepthStencilBuffer{};
		ComPtr<ID3D11DepthStencilView> m_DepthStencilView{};
		ComPtr<ID3D11DepthStencilState> m_DepthStencilState{};
		//ComPtr<ID3D11DepthStencilState> m_DepthDisabledStencilState{};

		// rasterizer state
		//ComPtr<ID3D11RasterizerState> m_RasterizerState{};
		// viewport
		D3D11_VIEWPORT m_ScreenViewport{};


		/** DirectX Math */
		DirectX::XMMATRIX m_Projection{};
	};
}