#include <Core/EntryPoint.h>

#include <Platform/D3D11/Drawable/D3D11DrawableGeomety.h>
#include <Platform/D3D11/Drawable/Light/PointLight.h>
#include <Camera/Camera.h>
#include <ModelLoader/Model.h>

#include <Vendor/ImGui/imgui.h>

using namespace Devil;
using namespace DirectX;

class ProvingGround : public Application
{
public:
	ProvingGround()
		: Application()
	{
		/*********/
		/** TEST */
		/*********/

		/** Init Window */
		m_Window = std::make_unique<Window>();
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		/** Init Camera */
		m_Camera = std::make_unique<Camera>();
		m_Camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -8.0f));


		/** Init Entity */
		m_Plane = std::make_unique<D3D11DrawableGeometry_BlinnPhongPosNorTex>(m_Window->GetRenderer(), DrawableGeometryType::Plane);
		m_Sphere = std::make_unique<D3D11DrawableGeometry_BlinnPhongPosNorTex>(m_Window->GetRenderer(), DrawableGeometryType::Sphere);
		m_Box = std::make_unique<D3D11DrawableGeometry_BlinnPhongPosNorTex>(m_Window->GetRenderer(), DrawableGeometryType::Box);
		m_Plane->SetPosition(XMFLOAT3(0.0f, -5.0f, 0.0f));
		m_Plane->SetScale(XMFLOAT3(2.0f, 1.0f, 2.0f));
		m_Sphere->SetPosition(XMFLOAT3(3.0f, 0.0f, 0.0f));
		m_Box->SetPosition(XMFLOAT3(-3.0f, 0.0f, 0.0f));
		m_Plane->SetTexture(m_Window->GetRenderer(), "./content/textures/wood.png");
		m_Sphere->SetTexture(m_Window->GetRenderer(), "./content/textures/brickwall.jpg");

		/** Init Model */
		m_AssimpTestModel = std::make_unique<Model>(m_Window->GetRenderer(),
			"./content/models/nanosuit/nanosuit.obj");
		m_AssimpTestModel->SetPosition(XMFLOAT3(0.0f, -3.0f, 0.0f));
		m_AssimpTestModel->SetRotation(XMFLOAT3(0.0f, 160.0f, 0.0f));
		m_AssimpTestModel->SetScale(XMFLOAT3(0.4f, 0.4f, 0.4f));

		/** Init Light */
		m_PointLight = std::make_unique<PointLight>(m_Window->GetRenderer(), m_Camera->GetPosition());
		m_PointLight->SetPosition(XMFLOAT3(-2.0f, 2.2f, -4.0f));



		/**********************/
		/** Projection Matrix */
		/**********************/
		m_Window->GetRenderer().SetProjection(DirectX::XMMatrixPerspectiveFovLH(45.0f, 
			(float)m_Window->GetWidth()/(float)m_Window->GetHeight(), 0.5f, 1000.0f));
	}

	virtual ~ProvingGround()
	{
	}

	virtual void DoFrame() override
	{
		// [Get Delta Time]
		float deltaTime = m_Timer.DeltaTime();


		/****************/
		/** Input Logic */
		/****************/
		// Camera Move Control
		if (!m_Window->IsCursorEnabled())
		{
			if (m_Window->KeyIsPressed('W'))
			{
				m_Camera->Translate(XMFLOAT3(0.0f, 0.0f, m_Camera->GetMoveSpeed() * deltaTime));
			}
			if (m_Window->KeyIsPressed('S'))
			{
				m_Camera->Translate(XMFLOAT3(0.0f, 0.0f, -m_Camera->GetMoveSpeed() * deltaTime));
			}
			if (m_Window->KeyIsPressed('A'))
			{
				m_Camera->Translate(XMFLOAT3(-m_Camera->GetMoveSpeed() * deltaTime, 0.0f, 0.0f));
			}
			if (m_Window->KeyIsPressed('D'))
			{
				m_Camera->Translate(XMFLOAT3(m_Camera->GetMoveSpeed() * deltaTime, 0.0f, 0.0f));
			}
			if (m_Window->KeyIsPressed('E'))
			{
				m_Camera->Translate(XMFLOAT3(0.0f, m_Camera->GetMoveSpeed() * deltaTime, 0.0f));
			}
			if (m_Window->KeyIsPressed('Q'))
			{
				m_Camera->Translate(XMFLOAT3(0.0f, -m_Camera->GetMoveSpeed() * deltaTime, 0.0f));
			}

			// Camera View Control
			while (const auto delta = m_Window->ReadRawDelta())
			{
				m_Camera->Rotate(delta->y * m_Camera->GetSensitivity() * deltaTime,
					delta->x * m_Camera->GetSensitivity() * deltaTime);
			}
		}


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
		/** 咀葎俶勣耽屐厚仟高坿佚連                             */
		/*******************************************************/
		m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_Plane.get(), 0u);
		m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_Sphere.get(), 0u);
		m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_Box.get(), 0u);
		for (int i = 0; i < m_AssimpTestModel->m_MeshPtrs.size(); i++)
		{
			m_PointLight->AddBindToDrawable(m_Window->GetRenderer(), m_AssimpTestModel->m_MeshPtrs[i].get(), 0u);
		}
		// ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！


		// Update && Draw Object
		m_Plane->Update(deltaTime);
		m_Sphere->Update(deltaTime);
		m_Box->Update(deltaTime);
		m_Plane->Draw(m_Window->GetRenderer());
		m_Sphere->Draw(m_Window->GetRenderer());
		m_Box->Draw(m_Window->GetRenderer());

		m_AssimpTestModel->Draw(m_Window->GetRenderer());


		/****************************************/
		/** clean up                            */
		/** 契峭坪贋亶其��効貧中議耽屐厚仟高坿斤哘 */
		/****************************************/
		m_Plane->CleanupFrameBinds();
		m_Sphere->CleanupFrameBinds();
		m_Box->CleanupFrameBinds();
		m_PointLight->GetMesh().CleanupFrameBinds();
		for (int i = 0; i < m_AssimpTestModel->m_MeshPtrs.size(); i++)
		{
			m_AssimpTestModel->m_MeshPtrs[i].get()->CleanupFrameBinds();
		}
		// ！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！


		// UI
		/****************/
		/** ImGui Layer */
		/****************/
		if (ImGui::Begin("Control Introductions"))
		{
			ImGui::Text("Left Shift + F1: Free Cursor");
			ImGui::Text("Alt + G: Confine/Lock Cursor");
		}
		ImGui::End();
		if (ImGui::Begin("Performance Profiler"))
		{
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		ImGui::End();
		// window raw input window.
		m_Window->ShowRawInputImGuiWindow();
		// camera imgui controll window.
		m_Camera->SpawnImGuiControlWindow();
		// light imgui controll window.
		m_PointLight->SpawnImGuiControlWindow(m_Window->GetRenderer(), m_Camera->GetPosition());

		m_Window->GetRenderer().EndFrame();
	}

protected:

	/***********/
	/** Entity */
	/***********/

	// model
	std::unique_ptr<Model> m_AssimpTestModel{};
	// geometry
	std::unique_ptr<D3D11DrawableGeometry_BlinnPhongPosNorTex> m_Plane{};
	std::unique_ptr<D3D11DrawableGeometry_BlinnPhongPosNorTex> m_Sphere{};
	std::unique_ptr<D3D11DrawableGeometry_BlinnPhongPosNorTex> m_Box{};
	// light
	std::unique_ptr<PointLight> m_PointLight{};
};




/***********************/
/** Create Application */
/***********************/
Devil::Application* Devil::CreateApplication()
{
	return new ProvingGround();
}