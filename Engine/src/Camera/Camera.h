#pragma once

#include <DirectXMath.h>

namespace Devil
{
	class Camera
	{
	public:
		Camera();
		Camera(float distance, float theta, float phi, float pitch, float yaw, float roll);
		virtual ~Camera() = default;

	public:
		DirectX::XMMATRIX GetViewMatrix() const noexcept;

		// imgui window
		void SpawnControllWindow();
		void Reset();

	private:
		float m_Distance{};	// Distance to the origin of the world
		float m_Theta{};	// Think of it as the equator ( ³àµÀ )
		float m_Phi{};		// It's like longitude ( ¾­¶È )

		float m_Pitch{};
		float m_Yaw{};
		float m_Roll{};
	};
}