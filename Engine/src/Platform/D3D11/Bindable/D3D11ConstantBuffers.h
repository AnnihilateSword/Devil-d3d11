#pragma once

#include "D3D11Bindable.h"

namespace Devil
{
	template<class T>
	class D3D11ConstantBuffers : public D3D11Bindable
	{
	public:
		D3D11ConstantBuffers(D3D11Renderer& renderer, const T& consts, unsigned int slot = 0u)
			: m_Slot{ slot }
		{
			D3D11_BUFFER_DESC bd{};
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.ByteWidth = sizeof(consts);
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0u;
			bd.StructureByteStride = 0u;
			bd.Usage = D3D11_USAGE_DYNAMIC;

			D3D11_SUBRESOURCE_DATA sd{};
			sd.pSysMem = &consts;

			// Has subresource data.
			GetDevice(renderer)->CreateBuffer(&bd, &sd, &m_ConstantBuffer);
		}

		D3D11ConstantBuffers(D3D11Renderer& renderer, unsigned int slot = 0u)
			: m_Slot{ slot }
		{
			D3D11_BUFFER_DESC bd{};
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.ByteWidth = sizeof(T);
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0u;
			bd.StructureByteStride = 0u;
			bd.Usage = D3D11_USAGE_DYNAMIC;

			// No subresource data.
			GetDevice(renderer)->CreateBuffer(&bd, nullptr, &m_ConstantBuffer);
		}


		/****************************/
		/** Update Constant Buffer. */
		/****************************/
		void Update(D3D11Renderer& renderer, const T& consts)
		{
			D3D11_MAPPED_SUBRESOURCE msr{};
			GetDeviceContext(renderer)->Map(m_ConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
			memcpy(msr.pData, &consts, sizeof(consts));
			GetDeviceContext(renderer)->Unmap(m_ConstantBuffer.Get(), 0u);
		}
		
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer{};
		unsigned int m_Slot{};
	};



	/**********************/
	/** VS Contant Buffer */
	/**********************/
	template<class T>
	class D3D11VSConstantBuffer : public D3D11ConstantBuffers<T>
	{
		using D3D11ConstantBuffers<T>::m_ConstantBuffer;
		using D3D11ConstantBuffers<T>::m_Slot;
		using D3D11Bindable::GetDeviceContext;
	public:
		using D3D11ConstantBuffers<T>::D3D11ConstantBuffers;
		virtual void Bind(D3D11Renderer& renderer) noexcept override
		{
			GetDeviceContext(renderer)->VSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};



	/**********************/
	/** PS Contant Buffer */
	/**********************/
	template<class T>
	class D3D11PSConstantBuffer : public D3D11ConstantBuffers<T>
	{
		using D3D11ConstantBuffers<T>::m_ConstantBuffer;
		using D3D11ConstantBuffers<T>::m_Slot;
		using D3D11Bindable::GetDeviceContext;
	public:
		using D3D11ConstantBuffers<T>::D3D11ConstantBuffers;
		virtual void Bind(D3D11Renderer& renderer) noexcept override
		{
			GetDeviceContext(renderer)->PSSetConstantBuffers(m_Slot, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};
}