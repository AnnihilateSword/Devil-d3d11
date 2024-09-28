#include "EnginePCH.h"

#include "D3D11Topology.h"

namespace Devil
{
	void D3D11Topology::Bind(D3D11Renderer& renderer) noexcept
	{
		GetDeviceContext(renderer)->IASetPrimitiveTopology(m_Type);
	}
}