#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	class D3D11IndexBuffer : public D3D11Bindable
	{
	public:
		D3D11IndexBuffer(D3D11Renderer& renderer, const std::vector<unsigned int>& indices);

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;
		unsigned int GetCount() const noexcept;

	private:
		unsigned int m_Count{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer{};
	};
}