#pragma once

#include "D3D11DrawableBase.h"

namespace Devil
{
	class D3D11Plane : public D3D11DrawableBase<D3D11Plane>
	{
	public:
		D3D11Plane(D3D11Renderer& renderer, DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	public:
		void Update(float deltaTime) noexcept override;

		/** Getter and Setter */
		void SetPosition(DirectX::XMFLOAT3& position) noexcept { m_Position = position; };
		void SetRotation(DirectX::XMFLOAT3& rotation) noexcept { m_Rotation = rotation; };
		void SetScale(DirectX::XMFLOAT3& scale) noexcept { m_Scale = scale; };

		/** World Transform */
		DirectX::XMMATRIX GetTransformXM() const noexcept override;

	private:
		DirectX::XMFLOAT3 m_Position{};
		DirectX::XMFLOAT3 m_Rotation{};
		DirectX::XMFLOAT3 m_Scale{};
	};
}