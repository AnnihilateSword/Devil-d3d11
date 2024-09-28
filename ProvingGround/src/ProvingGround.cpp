#include <Core/EntryPoint.h>

#include "Platform/D3D11/Drawable/D3D11Box.h"
#include "Platform/D3D11/Drawable/D3D11Sphere.h"
#include "Platform/D3D11/Drawable/D3D11Plane.h"

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


		/** Init Entity */
		m_Plane = std::make_unique<D3D11Plane>(m_Window->GetRenderer(),
			XMFLOAT3(0.0, -3.0f, 8.0f), XMFLOAT3(0.0, 0.0f, 0.0f), XMFLOAT3(1.0, 1.0f, 1.0f)
		);
		m_Box = std::make_unique<D3D11Box>(m_Window->GetRenderer(),
				XMFLOAT3(2.2, 0.0f, 6.0f), XMFLOAT3(0.0, 0.0f, 0.0f), XMFLOAT3(1.0, 1.0f, 1.0f)
			);
		m_Sphere = std::make_unique<D3D11Sphere>(m_Window->GetRenderer(),
			XMFLOAT3(-2.2, 0.0f, 6.0f), XMFLOAT3(0.0, 0.0f, 0.0f), XMFLOAT3(1.0, 1.0f, 1.0f)
		);


		m_Window->GetRenderer().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
	}

	virtual ~ProvingGround()
	{
	}

	virtual void DoFrame() override
	{
		// [Get Delta Time]
		float deltaTime = timer.DeltaTime();


		/******************************/
		/** Update && Draw && Present */
		/******************************/
		m_Window->GetRenderer().ClearBuffer(0.0f, 0.0f, 0.0f);
		
		m_Plane->Update(deltaTime);
		m_Plane->Draw(m_Window->GetRenderer());

		static float pitch = 0;
		static float yaw = 0;
		pitch += 0.6f * deltaTime;
		yaw += 1.2f * deltaTime;
		m_Box->SetRotation(XMFLOAT3(pitch, yaw, 0.0f));
		m_Box->Update(deltaTime);
		m_Box->Draw(m_Window->GetRenderer());

		m_Sphere->SetRotation(XMFLOAT3(pitch, yaw, 0.0f));
		m_Sphere->Update(deltaTime);
		m_Sphere->Draw(m_Window->GetRenderer());

		m_Window->GetRenderer().EndFrame();
	}

protected:
	// Entity
	std::unique_ptr<D3D11Plane> m_Plane{};
	std::unique_ptr<D3D11Box> m_Box{};
	std::unique_ptr<D3D11Sphere> m_Sphere{};
};




/***********************/
/** Create Application */
/***********************/
Devil::Application* Devil::CreateApplication()
{
	return new ProvingGround();
}