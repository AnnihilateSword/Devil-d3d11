#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11PixelShader : public D3D11Bindable
	{
	public:
		D3D11PixelShader(D3D11Renderer& renderer, std::wstring filename)
		{
			Microsoft::WRL::ComPtr<ID3DBlob> blob{};
			// Read file to blob.
			D3DReadFileToBlob(filename.c_str(), &blob);
			GetDevice(renderer)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(),
				nullptr, &m_PixelShader);
		}

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader{};
	};
}