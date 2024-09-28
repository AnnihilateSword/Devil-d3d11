#pragma once

#include "Platform/D3D11/D3D11Renderer.h"

namespace Devil
{
	class D3D11Bindable
	{
	public:
		D3D11Bindable() = default;
		virtual ~D3D11Bindable() = default;

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept = 0;

		static ID3D11Device* GetDevice(D3D11Renderer& renderer) noexcept;
		static ID3D11DeviceContext* GetDeviceContext(D3D11Renderer& renderer) noexcept;
	};
}