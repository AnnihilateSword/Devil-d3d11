#pragma once

#include "D3D11Bindable.h"


namespace Devil
{
	class D3D11VertexBuffer : public D3D11Bindable
	{
	public:
		template<class T>
		D3D11VertexBuffer(D3D11Renderer& renderer, const std::vector<T> vertices)
			: m_Stride{ sizeof(T) }
		{
			D3D11_BUFFER_DESC bd{};
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.ByteWidth = sizeof(T) * vertices.size();
			bd.CPUAccessFlags = 0u;
			bd.MiscFlags = 0u;
			bd.StructureByteStride = sizeof(T);
			bd.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = vertices.data();

			GetDevice(renderer)->CreateBuffer(&bd, &sd, &m_VertexBuffer);
		}

	public:
		virtual void Bind(D3D11Renderer& renderer) noexcept override;

	private:
		unsigned int m_Stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer{};
	};
}