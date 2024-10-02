#include "EnginePCH.h"

#include "Application.h"

namespace Devil
{
	Devil::Application::Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowEnterSizeMoveEvent>(std::bind(&Application::OnWindowEnterSizeMove, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowExitSizeMoveEvent>(std::bind(&Application::OnWindowExitSizeMove, this, std::placeholders::_1));

		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Application::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&Application::OnKeyReleased, this, std::placeholders::_1));

		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Application::OnMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Application::OnMouseButtonReleased, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&Application::OnMouseScrollerd, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Application::OnMouseMoved, this, std::placeholders::_1));
	}

	int Application::Run()
	{
		// Init Timer
		m_Timer.Reset();

		while (m_bRunning)
		{
			if (const std::optional<int> ecode = m_Window->OnUpdate())
			{
				return *ecode;
			}
			else
			{
				// Timer Tick
				m_Timer.Tick();

				if (!m_bAppPaused)
				{
					DoFrame();
				}
				else
				{
					Sleep(100);
				}
			}

		}

		return 0;
	}

	void Application::DoFrame()
	{
	}

	
	// *************************
	// Application Event Handler
	// *************************
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (!m_bResizing)
		{
			m_Window->SetWidth(e.GetWidth());
			m_Window->SetHeight(e.GetHeight());
			m_Window->GetRenderer().SetClientWidth(e.GetWidth());
			m_Window->GetRenderer().SetClientHeight(e.GetHeight());


			/**********************/
			/** Projection Matrix */
			/**********************/
			m_Window->GetRenderer().SetProjection(DirectX::XMMatrixPerspectiveFovLH(45.0f, 
				(float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.5f, 1000.0f));

			m_Window->GetRenderer().OnResize();
		}

		return true;
	}

	bool Application::OnWindowEnterSizeMove(WindowEnterSizeMoveEvent& e)
	{
		m_bAppPaused = true;
		m_bResizing = true;
		m_Timer.Stop();
		return true;
	}

	bool Application::OnWindowExitSizeMove(WindowExitSizeMoveEvent& e)
	{
		m_bAppPaused = false;
		m_bResizing = false;
		m_Timer.Start();
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());

		return true;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());
		return true;
	}

	bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());
		return true;
	}

	bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());
		return true;
	}

	bool Application::OnMouseScrollerd(MouseScrolledEvent& e)
	{
		OutputDebugString(e.ToString().c_str());
		return true;
	}

	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());
		return false;
	}
}