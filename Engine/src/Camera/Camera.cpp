#include "EnginePCH.h"

#include "Camera.h"
#include "Core/Core.h"

#include <Vendor/ImGui/imgui.h>

using namespace DirectX;

namespace Devil
{
	Camera::Camera()
		: m_Distance{ 10.0f }, m_Theta{ DegreeToRadian(0) }, m_Phi{ DegreeToRadian(0) },
		m_Pitch{ DegreeToRadian(0) }, m_Yaw{ DegreeToRadian(0) }, m_Roll{ DegreeToRadian(0) }
	{}

	Camera::Camera(float distance, float thetaDegree, float phiDegree, float pitchDegree, float yawDegree, float rollDegree)
		: m_Distance{ distance }, m_Theta{ DegreeToRadian(thetaDegree) }, m_Phi{ DegreeToRadian(phiDegree) },
		m_Pitch{ DegreeToRadian(pitchDegree) }, m_Yaw{ DegreeToRadian(yawDegree) }, m_Roll{ DegreeToRadian(rollDegree) }
	{}

	DirectX::XMMATRIX Camera::GetViewMatrix() const noexcept
	{
		// Around the world origin
		const auto pos = XMVector3Transform(
			XMVectorSet(0.0f, 0.0f, -m_Distance, 0.0f),
			XMMatrixRotationRollPitchYaw(DegreeToRadian(m_Phi), -DegreeToRadian(m_Theta), 0.0f)
		);

		return XMMatrixLookAtLH(pos, XMVectorZero(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
			XMMatrixRotationRollPitchYaw(DegreeToRadian(m_Pitch), -DegreeToRadian(m_Yaw), DegreeToRadian(m_Roll));
	}

	void Camera::SpawnControllWindow()
	{
		//m_Theta += 0.001f;
		if (ImGui::Begin("Camera Control Window"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("m_Distance", &m_Distance, 0.1f, 80.0f, "%.1f");
			ImGui::SliderFloat("m_Theta", &m_Theta, -180.0f, 180.0f);
			ImGui::SliderFloat("m_Phi", &m_Phi, -89.0f, 89.0f);
			ImGui::Text("Orientation");
			ImGui::SliderFloat("m_Pitch", &m_Pitch, -180.0f, 180.0f);
			ImGui::SliderFloat("m_Yaw", &m_Yaw, -180.0f, 180.0f);
			ImGui::SliderFloat("m_Roll", &m_Roll, -180.0f, 180.0f);
			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}
		ImGui::End();
	}

	void Camera::Reset()
	{
		m_Distance = 10.0f;
		m_Theta = DegreeToRadian(0);
		m_Phi = DegreeToRadian(0);
		m_Pitch = DegreeToRadian(0);
		m_Yaw = DegreeToRadian(0);
		m_Roll = DegreeToRadian(0);
	}
}