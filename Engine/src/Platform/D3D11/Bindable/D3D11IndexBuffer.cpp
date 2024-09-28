#include "EnginePCH.h"

#include "D3D11IndexBuffer.h"

namespace Devil
{
	D3D11IndexBuffer::D3D11IndexBuffer(D3D11Renderer& renderer, const std::vector<unsigned int>& indices)
		: m_Count{ static_cast<unsigned int>(indices.size()) }
	{
		D3D11_BUFFER_DESC bd{};
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(unsigned int) * indices.size();
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizeof(unsigned int);
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd{};
		sd.pSysMem = indices.data();

		GetDevice(renderer)->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	void D3D11IndexBuffer::Bind(D3D11Renderer& renderer) noexcept
	{
		GetDeviceContext(renderer)->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	unsigned int D3D11IndexBuffer::GetCount() const noexcept
	{
		return m_Count;
	}
}