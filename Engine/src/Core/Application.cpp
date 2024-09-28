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
		timer.Reset();

		while (m_Running)
		{
			if (const std::optional<int> ecode = m_Window->OnUpdate())
			{
				return *ecode;
			}

			// Timer Tick
			timer.Tick();

			DoFrame();
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
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
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