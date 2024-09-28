#include "EnginePCH.h"

#include "D3D11InputLayout.h"

namespace Devil
{
	void D3D11InputLayout::Bind(D3D11Renderer& renderer) noexcept
	{
		GetDeviceContext(renderer)->IASetInputLayout(m_InputLayout.Get());
	}
}
