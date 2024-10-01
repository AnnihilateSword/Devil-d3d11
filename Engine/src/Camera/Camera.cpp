#include "EnginePCH.h"

#include "Camera.h"
#include "Core/Core.h"

#include <Vendor/ImGui/imgui.h>

using namespace DirectX;

namespace Devil
{
	Camera::Camera(DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& rotation)
		: DObject(position, rotation)
	{}

	DirectX::XMMATRIX Camera::GetViewMatrix() const noexcept
	{
		XMFLOAT3 focusPostion(m_Position.x, m_Position.y, m_Position.z + 1.0f);
		return XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&focusPostion), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
			XMMatrixRotationRollPitchYaw(DegreeToRadian(m_Rotation.x), -DegreeToRadian(m_Rotation.y), DegreeToRadian(m_Rotation.z));
	}

	void Camera::SpawnImGuiControlWindow()
	{
		//m_Theta += 0.001f;
		if (ImGui::Begin("Camera Control Window"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("x", &m_Position.x, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("y", &m_Position.y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("z", &m_Position.z, -80.0f, 80.0f, "%.1f");
			ImGui::Text("Rotation");
			ImGui::SliderFloat("m_Pitch", &m_Rotation.x, -180.0f, 180.0f);
			ImGui::SliderFloat("m_Yaw", &m_Rotation.y, -180.0f, 180.0f);
			ImGui::SliderFloat("m_Roll", &m_Rotation.z, -180.0f, 180.0f);
			if (ImGui::Button("Reset"))
			{
				Reset();
			}
		}
		ImGui::End();
	}

	void Camera::Reset()
	{
		m_Position = XMFLOAT3(0.0f, 0.0f, -5.0f);
		m_Rotation = XMFLOAT3(DegreeToRadian(0.0f), DegreeToRadian(0.0f), DegreeToRadian(0.0f));
		m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	}
}