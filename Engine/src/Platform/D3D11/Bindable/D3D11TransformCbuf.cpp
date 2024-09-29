#include "EnginePCH.h"

#include "D3D11TransformCbuf.h"

using namespace DirectX;

namespace Devil
{
	D3D11TransformCbuf::D3D11TransformCbuf(D3D11Renderer& renderer, const D3D11Drawable& parent)
		: m_Parent{ parent }
	{
		if (s_VCbuf == nullptr)
		{
			s_VCbuf = std::make_unique<D3D11VSConstantBuffer<DirectX::XMMATRIX>>(renderer);
		}
	}

	void D3D11TransformCbuf::Bind(D3D11Renderer& renderer) noexcept
	{
		s_VCbuf->Update(renderer,
			// mvp
			XMMatrixTranspose(m_Parent.GetTransformXM() *
			renderer.GetCameraMatrix() *
			renderer.GetProjection())
		);
		s_VCbuf->Bind(renderer);
	}

	std::unique_ptr<D3D11VSConstantBuffer<DirectX::XMMATRIX>> D3D11TransformCbuf::s_VCbuf{};
}