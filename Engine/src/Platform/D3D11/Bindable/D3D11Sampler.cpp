#include "EnginePCH.h"

#include "D3D11Sampler.h"

namespace Devil
{
	D3D11Sampler::D3D11Sampler(D3D11Renderer& renderer)
	{
		D3D11_SAMPLER_DESC sd{};
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		GetDevice(renderer)->CreateSamplerState(&sd, &m_Sampler);
	}

	void D3D11Sampler::Bind(D3D11Renderer& renderer) noexcept
	{
		GetDeviceContext(renderer)->PSSetSamplers(0u, 1u, m_Sampler.GetAddressOf());
	}
}