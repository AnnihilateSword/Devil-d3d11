#pragma once

#include "D3D11Bindable.h"
#include "D3D11ConstantBuffers.h"
#include "../Drawable/D3D11Drawable.h"

namespace Devil
{
	class D3D11TransformCbuf : public D3D11Bindable
	{
	public:
		D3D11TransformCbuf(D3D11Renderer& renderer, const D3D11Drawable& parent);

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		struct Transforms
		{
			DirectX::XMMATRIX model;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX proj;
			DirectX::XMMATRIX modelInvTranspose;
		};

	private:
		// Use the same ConstantBuffer for the same object
		static std::unique_ptr<D3D11VSConstantBuffer<D3D11TransformCbuf::Transforms>> s_VCbuf;
		const D3D11Drawable& m_Parent;
	};
}