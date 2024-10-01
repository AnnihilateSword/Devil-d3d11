#include "EnginePCH.h"

#include "D3D11TransformCbuf.h"
#include "Utility/DevilLibrary.h"

using namespace DirectX;

namespace Devil
{
	D3D11TransformCbuf::D3D11TransformCbuf(D3D11Renderer& renderer, const D3D11Drawable& parent)
		: m_Parent{ parent }
	{
		if (s_VCbuf == nullptr)
		{
			s_VCbuf = std::make_unique<D3D11VSConstantBuffer<Transforms>>(renderer);
		}
	}

	void D3D11TransformCbuf::Bind(D3D11Renderer& renderer) noexcept
	{
		const auto model = m_Parent.GetTransformXM();
		const Transforms tf =
		{
			// mvp
			XMMatrixTranspose(model),
			XMMatrixTranspose(renderer.GetCameraMatrix()),
			XMMatrixTranspose(renderer.GetProjection()),
			XMMatrixTranspose(InverseTranspose(model))  // normal vector transform
		};
		s_VCbuf->Update(renderer, tf);
		s_VCbuf->Bind(renderer);
	}

	std::unique_ptr<D3D11VSConstantBuffer<D3D11TransformCbuf::Transforms>> D3D11TransformCbuf::s_VCbuf{};
}