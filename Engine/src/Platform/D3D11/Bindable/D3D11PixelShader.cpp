#include "EnginePCH.h"

#include "D3D11PixelShader.h"

namespace Devil
{
	void D3D11PixelShader::Bind(D3D11Renderer& renderer) noexcept
	{
		GetDeviceContext(renderer)->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	}
}