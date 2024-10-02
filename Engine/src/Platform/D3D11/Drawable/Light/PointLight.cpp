#include "EnginePCH.h"

#include "PointLight.h"

#include "Vendor/ImGui/imgui.h"

using namespace DirectX;

namespace Devil
{
	PointLight::PointLight(D3D11Renderer& renderer, XMFLOAT3& viewPos, DrawableGeometryType drawableType,
		XMFLOAT3& position, XMFLOAT3& rotation, XMFLOAT3& scale)
		: m_Mesh{ renderer, drawableType, position, rotation, scale }
	{
		m_CbData = {
		   position,
		   { 0.05f, 0.05f, 0.05f },
		   m_Mesh.GetMaterialColor().color,
		   1.0f,
		   1.0f,
		   0.045f,
		   0.0075f,
		   viewPos
		};
	}
	
	void PointLight::Draw(D3D11Renderer& renderer) noexcept
	{
		SetPosition(m_CbData.pos);
		m_Mesh.Draw(renderer);
	}

	void PointLight::Update(D3D11Renderer& renderer, float deltaTime) noexcept
	{
		m_Mesh.SetMaterialColor(renderer, m_CbData.diffuseColor);

		m_Mesh.Update(deltaTime);
	}

	void PointLight::Reset(D3D11Renderer& renderer, XMFLOAT3& viewPos) noexcept
	{
		m_CbData = {
		   { 0.0f, 0.0f, 0.0f },
		   { 0.05f, 0.05f, 0.05f },
		   { 1.0f, 1.0f, 1.0f },
		   1.0f,
		   1.0f,
		   0.045f,
		   0.0075f,
		   viewPos
		};

		m_Position = m_CbData.pos;
		m_Mesh.SetMaterialColor(renderer, m_CbData.diffuseColor);
	}

	void PointLight::SpawnImGuiControlWindow(D3D11Renderer& renderer, XMFLOAT3& viewPos) noexcept
	{
		if (ImGui::Begin("Light"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &m_CbData.pos.x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &m_CbData.pos.y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &m_CbData.pos.z, -60.0f, 60.0f, "%.1f");

			ImGui::Text("Intensity/Color");
			ImGui::SliderFloat("Intensity", &m_CbData.diffuseIntensity, 0.01f, 2.0f, "%.2f");
			ImGui::ColorEdit3("Diffuse Color", &m_CbData.diffuseColor.x);
			ImGui::ColorEdit3("Ambient", &m_CbData.ambient.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &m_CbData.attConst, 0.05f, 10.0f, "%.2f");
			ImGui::SliderFloat("Linear", &m_CbData.attLin, 0.0001f, 4.0f, "%.4f");
			ImGui::SliderFloat("Quadratic", &m_CbData.attQuad, 0.0000001f, 10.0f, "%.7f");

			if (ImGui::Button("Reset"))
			{
				Reset(renderer, viewPos);
			}
		}
		ImGui::End();
	}

	void PointLight::AddBindToDrawable(D3D11Renderer& renderer, D3D11Drawable* drawable, unsigned int slot) noexcept
	{
		// Prevent memory leaks!!!
		drawable->AddBindAndCleanupFrame(std::make_unique<D3D11PSConstantBuffer<PointLightCBuf>>(renderer, m_CbData, slot));
		// Prevent memory leaks!!!
	}

	void PointLight::SetPosition(DirectX::XMFLOAT3& position) noexcept
	{
		m_Position = position;
		m_Mesh.SetPosition(position);

		// update constant buffer
		m_CbData.pos = position;
	}
	
	void PointLight::SetRotation(DirectX::XMFLOAT3& rotation) noexcept
	{
		m_Rotation = rotation;
		m_Mesh.SetRotation(rotation);
	}

	void PointLight::SetScale(DirectX::XMFLOAT3& scale) noexcept
	{
		m_Scale = scale;
		m_Mesh.SetScale(scale);
	}
}