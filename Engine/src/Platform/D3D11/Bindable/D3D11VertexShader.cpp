#include "EnginePCH.h"

#include "D3D11VertexShader.h"

namespace Devil
{
	void D3D11VertexShader::Bind(D3D11Renderer& renderer) noexcept
	{
		GetDeviceContext(renderer)->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* D3D11VertexShader::GetBytecodeBlob() const noexcept
	{
		return m_BytecodeBlob.Get();
	}
}