#include "EnginePCH.h"

#include "D3D11Bindable.h"

namespace Devil
{
	ID3D11Device* D3D11Bindable::GetDevice(D3D11Renderer& renderer) noexcept
	{
		return renderer.m_Device.Get();
	}

	ID3D11DeviceContext* D3D11Bindable::GetDeviceContext(D3D11Renderer& renderer) noexcept
	{
		return renderer.m_DeviceContext.Get();
	}
}