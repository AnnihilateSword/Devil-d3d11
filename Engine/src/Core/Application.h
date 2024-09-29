#pragma once

#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "DevilTimer.h"


int WINAPI::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

namespace Devil
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

	public:
		virtual void OnEvent(Event& e);

		virtual int Run();

	protected:
		virtual void DoFrame();


		/** Event Handle */
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowEnterSizeMove(WindowEnterSizeMoveEvent& e);
		bool OnWindowExitSizeMove(WindowExitSizeMoveEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrollerd(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);

	protected:
		std::unique_ptr<Window> m_Window{};

		bool m_bRunning{ true };
		bool m_bMinimized{ false };


		// friend
		friend int WINAPI::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	protected:
		// Timer
		DevilTimer m_Timer;

	private:
		bool m_bResizing{ false };
		bool m_bAppPaused{ false };
	};


	/** Defined in the project. */
	Application* CreateApplication();
}