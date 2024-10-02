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
		dispatcher.Dispatch<WindowActivateEvent>(std::bind(&Application::OnWindowActivate, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowEnterSizeMoveEvent>(std::bind(&Application::OnWindowEnterSizeMove, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowExitSizeMoveEvent>(std::bind(&Application::OnWindowExitSizeMove, this, std::placeholders::_1));

		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Application::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&Application::OnKeyReleased, this, std::placeholders::_1));

		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Application::OnMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Application::OnMouseButtonReleased, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&Application::OnMouseScrollerd, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Application::OnMouseMoved, this, std::placeholders::_1));

		dispatcher.Dispatch<WindowRawInputEvent>(std::bind(&Application::OnWindowRawInput, this, std::placeholders::_1));
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
					Sleep(10);
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

	bool Application::OnWindowActivate(WindowActivateEvent& e)
	{
		// Ensure that the mouse is locked when Alt+Tab activates the window.
		if (!m_Window->IsCursorEnabled())
		{
			if (e.GetWParam() & WA_ACTIVE || e.GetWParam() & WA_CLICKACTIVE)
			{
				OutputDebugString("window activeate => confine\n");
				m_Window->DisableCursor();
				SetForegroundWindow(reinterpret_cast<HWND>(m_Window->GetNativeWindow()));
				SetFocus(reinterpret_cast<HWND>(m_Window->GetNativeWindow()));
			}
			else
			{
				OutputDebugString("window activeate => free\n");
				m_Window->DisableCursor();
			}
		}

		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
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

		return true;
	}

	bool Application::OnWindowEnterSizeMove(WindowEnterSizeMoveEvent& e)
	{
		m_bAppPaused = true;
		m_Timer.Stop();
		return true;
	}

	bool Application::OnWindowExitSizeMove(WindowExitSizeMoveEvent& e)
	{
		m_bAppPaused = false;
		m_Timer.Start();
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());

		/** Key Code State ( in window.cpp now ) */
		//m_Window->m_KeyStates[e.GetKeyCode()] = true;

		// left shift + f1 (free cursor)
		if (e.GetKeyCode() ==  VK_F1)
		{
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				if (!m_Window->IsCursorEnabled())
				{
					m_Window->EnableCursor();
					m_Window->DisableRawInput();
				}
			}
		}
		// alt + g (free cursor)
		if (e.GetKeyCode() == 'G')
		{
			if (GetKeyState(VK_MENU) & 0x8000)
			{
				if (m_Window->IsCursorEnabled())
				{
					m_Window->DisableCursor();
					m_Window->EnableRawInput();
				}
			}
		}
		// alt + f4 (close application)
		if (e.GetKeyCode() == VK_F4)
		{
			if (GetKeyState(VK_MENU) & 0x8000)
			{
				PostMessage(reinterpret_cast<HWND>(m_Window->GetNativeWindow()), WM_CLOSE, 0, 0);
			}
		}

		return true;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		OutputDebugString(e.ToString().c_str());

		/** Key Code State ( in window.cpp now ) */
		//m_Window->m_KeyStates[e.GetKeyCode()] = false;

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
		// cursorless exclusive gets first dibs
		if (!m_Window->IsCursorEnabled())
		{
			if (!m_Window->IsInWindow())
			{
				SetCapture(reinterpret_cast<HWND>((m_Window->GetNativeWindow())));
				m_Window->SetInWindow(true);
				m_Window->DisableCursor();
			}
			return true;
		}
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (e.GetX() >= 0 && e.GetX() < m_Window->GetWidth() && e.GetY() >= 0 && e.GetY() < m_Window->GetHeight())
		{
			if (!m_Window->IsInWindow())
			{
				SetCapture(reinterpret_cast<HWND>((m_Window->GetNativeWindow())));
				m_Window->SetInWindow(true);

				OutputDebugString(e.ToString().c_str());
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (e.GetWParam() & (MK_LBUTTON | MK_RBUTTON))
			{
				OutputDebugString(e.ToString().c_str());
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				m_Window->SetInWindow(false);
			}
		}

		return true;
	}

	bool Application::OnWindowRawInput(WindowRawInputEvent& e)
	{
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(e.GetLParam()),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			return false;
		}
		m_Window->m_RawBuffer.resize(size);
		// read in the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(e.GetLParam()),
			RID_INPUT,
			m_Window->m_RawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			// bail msg processing if error
			return false;
		}
		// process the raw input data
		auto& ri = reinterpret_cast<const RAWINPUT&>(*m_Window->m_RawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			m_Window->m_RawDeltaBuffer.push({ ri.data.mouse.lLastX, ri.data.mouse.lLastY });
		}

		return true;
	}
}