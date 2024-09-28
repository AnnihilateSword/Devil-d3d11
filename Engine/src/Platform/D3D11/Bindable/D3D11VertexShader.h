#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11VertexShader : public D3D11Bindable
	{
	public:
		D3D11VertexShader(D3D11Renderer& renderer, std::wstring filename)
		{
			// Read file to blob.
			D3DReadFileToBlob(filename.c_str(), &m_BytecodeBlob);
			GetDevice(renderer)->CreateVertexShader(m_BytecodeBlob->GetBufferPointer(), m_BytecodeBlob->GetBufferSize(), 
				nullptr, &m_VertexShader);
		}

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

		ID3DBlob* GetBytecodeBlob() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_BytecodeBlob{};
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader{};
	};
}