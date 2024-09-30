#include <Core/EntryPoint.h>

#include <Platform/D3D11/Drawable/D3D11DrawableGeomety.h>
#include <Platform/D3D11/Drawable/Light/PointLight.h>

#include <Vendor/ImGui/imgui.h>

using namespace Devil;
using namespace DirectX;

class ProvingGround : public Application
{
public:
	ProvingGround()
	{
		/*********/
		/** TEST */
		/*********/

		/** Init Window */
		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		/** Init Camera */
		m_Camera = std::make_unique<Camera>();
		m_Camera->SetDistanceToOrigin(10.0f);


		/** Init Entity */
		m_Plane = std::make_unique<D3D11DrawableGeometry_PhongPosNormalTex>(m_Window->GetRenderer(), DrawableGeometryType::Plane);
		m_Sphere = std::make_unique<D3D11DrawableGeometry_PhongPosNormalTex>(m_Window->GetRenderer(), DrawableGeometryType::Sphere);
		m_Box = std::make_unique<D3D11DrawableGeometry_PhongPosNormalTex>(m_Window->GetRenderer(), DrawableGeometryType::Box);
		m_Plane->SetPosition(XMFLOAT3(0.0f, -5.0f, 0.0f));
		m_Sphere->SetPosition(XMFLOAT3(3.0f, 0.0f, 0.0f));
		m_Box->SetPosition(XMFLOAT3(-3.0f, 0.0f, 0.0f));
		m_Plane->SetTexture(m_Window->GetRenderer(), "./content/textures/wood.png");
		m_Sphere->SetTexture(m_Window->GetRenderer(), "./content/textures/brickwall.jpg");

		/** Init Light */
		m_PointLight = std::make_unique<PointLight>(m_Window->GetRenderer());
		m_PointLight->SetPosition(XMFLOAT3(0.0f, 1.0f, -4.0f));

		m_Window->GetRenderer().SetProjection(DirectX::XMMatrixPerspectiveFovLH(45.0f, 
			(float)m_Window->GetWidth()/(float)m_Window->GetHeight(), 0.5f, 40.0f));
	}

	virtual ~ProvingGround()
	{
	}

	virtual void DoFrame() override
	{
		// [Get Delta Time]
		float deltaTime = m_Timer.DeltaTime();


		/******************************/
		/** Update && Draw && Present */
		/******************************/
		m_Window->GetRenderer().BeginFrame(0.0f, 0.0f, 0.0f);

		// Update camera view matrix
		m_Window->GetRenderer().SetCameraMatrix(m_Camera->GetViewMatrix());

		// Update && Draw Light
		m_PointLight->Update(m_Window->GetRenderer(), deltaTime);
		m_PointLight->Draw(m_Window->GetRenderer());
		
		/*******************************************************/
		/** Bind the light source to all objects in the scene. */
		/*******************************************************/
		m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_Plane.get(), 0u);
		m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_Sphere.get(), 0u);
		m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_Box.get(), 0u);

		// Update && Draw Object
		m_Plane->Update(deltaTime);
		m_Sphere->Update(deltaTime);
		m_Box->Update(deltaTime);
		m_Plane->Draw(m_Window->GetRenderer());
		m_Sphere->Draw(m_Window->GetRenderer());
		m_Box->Draw(m_Window->GetRenderer());


		/****************/
		/** ImGui Layer */
		/****************/
		if (ImGui::Begin("Performance Profiler"))
		{
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
		// camera imgui controll window.
		m_Camera->SpawnImGuiControlWindow();
		// light imgui controll window.
		m_PointLight->SpawnImGuiControlWindow(m_Window->GetRenderer());

		m_Window->GetRenderer().EndFrame();
	}

protected:
	// Entity
	std::unique_ptr<D3D11DrawableGeometry_PhongPosNormalTex> m_Sphere{};
	std::unique_ptr<D3D11DrawableGeometry_PhongPosNormalTex> m_Plane{};
	std::unique_ptr<D3D11DrawableGeometry_PhongPosNormalTex> m_Box{};
	std::unique_ptr<PointLight> m_PointLight{};
};




/***********************/
/** Create Application */
/***********************/
Devil::Application* Devil::CreateApplication()
{
	return new ProvingGround();
}