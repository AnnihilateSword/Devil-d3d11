#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11Topology : public D3D11Bindable
	{
	public:
		D3D11Topology(D3D11Renderer& renderer, D3D11_PRIMITIVE_TOPOLOGY type)
			: m_Type{ type } {}

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY m_Type{};
	};
}