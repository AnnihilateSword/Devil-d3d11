#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11Sampler : public D3D11Bindable
	{
	public:
		D3D11Sampler(D3D11Renderer& renderer);

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler{};
	};
}