#pragma once

#include <DirectXMath.h>

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