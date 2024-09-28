#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11InputLayout : public D3D11Bindable
	{
	public:
		D3D11InputLayout(D3D11Renderer& renderer, const D3D11_INPUT_ELEMENT_DESC* layout, unsigned int layoutSize, ID3DBlob* pVertexShaderBytecode)
		{
			GetDevice(renderer)->CreateInputLayout(layout, layoutSize,
				pVertexShaderBytecode->GetBufferPointer(), pVertexShaderBytecode->GetBufferSize(), &m_InputLayout);
		}

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout{};
	};
}