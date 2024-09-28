#include "EnginePCH.h"

#include "D3D11VertexBuffer.h"

namespace Devil
{
	void D3D11VertexBuffer::Bind(D3D11Renderer& renderer) noexcept
	{
		const unsigned int offset{ 0 };
		GetDeviceContext(renderer)->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &m_Stride, &offset);
	}
}