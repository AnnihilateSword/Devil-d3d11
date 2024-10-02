#pragma once

#include <DirectXMath.h>

#include "Platform/D3D11/Object/DObject.h"

namespace Devil
{
	class Camera : public DObject
	{
	public:
		Camera(DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		virtual ~Camera() = default;

	public:
		/** Logic Functions */
		void Rotate(float pitch, float yaw);

	public:
		/** Setter and Getter */
		void SetMoveSpeed(float moveSpeed) noexcept { m_MoveSpeed = moveSpeed; }
		void SetSensitivity(float sensitivity) noexcept { m_Sensitivity = sensitivity; }

		DirectX::XMMATRIX GetViewMatrix() const noexcept;
		float GetMoveSpeed() const noexcept { return m_MoveSpeed; }
		float GetSensitivity() const noexcept { return m_Sensitivity; }

		// imgui window
		void SpawnImGuiControlWindow();
		void Reset();

	public:
		float m_MoveSpeed{ 9.0f };
		float m_Sensitivity{ 32.0f };
	};
}