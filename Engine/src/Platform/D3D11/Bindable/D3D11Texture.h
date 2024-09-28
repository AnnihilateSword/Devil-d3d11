#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11Texture : public D3D11Bindable
	{
	public:
		D3D11Texture(D3D11Renderer& renderer, const std::string filename);

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView{};
	};
}