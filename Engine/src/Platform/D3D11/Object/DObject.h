#pragma once

#include <DirectXMath.h>

#include "Core/Core.h"

namespace Devil
{
	class DObject
	{
	public:
		DObject(DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
			: m_Position{ position }, m_Rotation{ rotation }, m_Scale{ scale }
		{
		}

	public:
		/** Logic Functions */
		void Translate(DirectX::XMFLOAT3& translation)
		{
			DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
				DirectX::XMLoadFloat3(&translation),
				DirectX::XMMatrixRotationRollPitchYaw(DegreeToRadian(m_Rotation.x), DegreeToRadian(m_Rotation.y), 0.0f)
			));
			m_Position = {
				m_Position.x + translation.x,
				m_Position.y + translation.y,
				m_Position.z + translation.z
			};
		}

	public:
		// Setter && Getter
		virtual void SetPosition(DirectX::XMFLOAT3& position) noexcept { m_Position = position; };
		virtual void SetRotation(DirectX::XMFLOAT3& rotation) noexcept { m_Rotation = rotation; };
		virtual void SetScale(DirectX::XMFLOAT3& scale) noexcept { m_Scale = scale; };

		DirectX::XMFLOAT3& GetPosition() noexcept { return m_Position; };
		DirectX::XMFLOAT3& GetRotation() noexcept { return m_Rotation; };
		DirectX::XMFLOAT3& GetScale() noexcept { return m_Scale; };

	protected:
		// Transform
		DirectX::XMFLOAT3 m_Position{};
		DirectX::XMFLOAT3 m_Rotation{};
		DirectX::XMFLOAT3 m_Scale{};
	};
}