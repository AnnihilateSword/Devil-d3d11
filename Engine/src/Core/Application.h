#pragma once

#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "DevilTimer.h"
#include "ImGui/ImGuiManager.h"
#include "Camera/Camera.h"


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
		virtual bool OnWindowClose(WindowCloseEvent& e);
		virtual bool OnWindowActivate(WindowActivateEvent& e);
		virtual bool OnWindowResize(WindowResizeEvent& e);
		virtual bool OnWindowEnterSizeMove(WindowEnterSizeMoveEvent& e);
		virtual bool OnWindowExitSizeMove(WindowExitSizeMoveEvent& e);
		virtual bool OnKeyPressed(KeyPressedEvent& e);
		virtual bool OnKeyReleased(KeyReleasedEvent& e);
		virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		virtual bool OnMouseScrollerd(MouseScrolledEvent& e);
		virtual bool OnMouseMoved(MouseMovedEvent& e);
		virtual bool OnWindowRawInput(WindowRawInputEvent& e);

	protected:
		std::unique_ptr<Window> m_Window{};
		std::unique_ptr<Camera> m_Camera{};

		bool m_bRunning{ true };
		bool m_bMinimized{ false };


		// friend
		friend int WINAPI::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	protected:
		// Timer
		DevilTimer m_Timer;

		// ImGui Manager
		ImGuiManager m_ImGuiManager;

	private:
		bool m_bAppPaused{ false };
	};


	/** Defined in the project. */
	Application* CreateApplication();
}