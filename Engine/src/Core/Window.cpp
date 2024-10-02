#include "EnginePCH.h"

#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "../resource.h"

#include <Vendor/ImGui/imgui_impl_win32.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Devil
{
    // ********************************
    // Devil Window Exception Functions
    // ********************************
    Window::Exception::Exception(const char* file, int line, HRESULT hr) noexcept
        : DevilException(file, line), m_Hr{ hr } {}

    const char* Window::Exception::what() const noexcept
    {
        std::ostringstream oss;
        oss << GetType() << std::endl
            << "[Error Code]: " << GetErrorCode() << std::endl
            << "[Description]: " << GetErrorString() << std::endl
            << GetOriginString();
        m_Whatbuffer = oss.str();
        return m_Whatbuffer.c_str();
    }
    const char* Window::Exception::GetType() const noexcept
    {
        return "Devil Window Exception";
    }
    std::string Window::Exception::TranslateErrorCode(HRESULT hr)
    {
        char* pMsgBuf{ nullptr };
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
            nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
        if (pMsgBuf == nullptr)
        {
            return "Unidentified error code";
        }
        std::string errorString = pMsgBuf;
        LocalFree(pMsgBuf);
        return errorString;
    }
    HRESULT Window::Exception::GetErrorCode() const noexcept
    {
        return m_Hr;
    }
    std::string Window::Exception::GetErrorString() const noexcept
    {
        return TranslateErrorCode(m_Hr);
    }


    // **********************
    // WindowClass Functions
    // **********************
    Window::WindowClass Window::WindowClass::s_WndClass;

    Window::WindowClass::WindowClass() noexcept
        : m_hInstance{ GetModuleHandle(nullptr) }
    {
        WNDCLASSEX cx{};
        cx.cbClsExtra = 0;
        cx.cbSize = sizeof(cx);
        cx.cbWndExtra = 0;
        cx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  // Avoid initial white screen
        cx.hCursor = nullptr;
        cx.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 256, 256, 0));
        cx.hIconSm = cx.hIcon;
        cx.hInstance = GetInstance();
        cx.lpfnWndProc = HandleMsgSetup;
        cx.lpszClassName = GetName();
        cx.lpszMenuName = nullptr;
        cx.style = CS_OWNDC;

        RegisterClassEx(&cx);
    }

    Window::WindowClass::~WindowClass()
    {
        UnregisterClass(s_WndClassName, GetInstance());
    }

    const char* Window::WindowClass::GetName() noexcept
    {
        return s_WndClassName;
    }

    HINSTANCE Window::WindowClass::GetInstance() noexcept
    {
        return s_WndClass.m_hInstance;
    }



    // *****************
    // Window Functions
    // *****************
	Window::Window(const WindowProps& props)
	{
        RECT clientRect{ 0, 0, (long)props.Width, (long)props.Height };
        if (FAILED(AdjustWindowRectEx(&clientRect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW)))
        {
            throw DEVIL_LAST_EXCEPT();
        }

        m_Hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
            WindowClass::GetName(), props.Title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            clientRect.right - clientRect.left,
            clientRect.bottom - clientRect.top,
            nullptr, nullptr, WindowClass::GetInstance(), this /** use in WM_NCCREATE */
        );
        // check for error
        if (m_Hwnd == nullptr)
        {
            throw DEVIL_LAST_EXCEPT();
        }

        // Init Window Data
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.Title = props.Title;

        ShowWindow(m_Hwnd, SW_SHOW);


        /** ImGui Layer */
        // Setup Platform backends
        ImGui_ImplWin32_Init(m_Hwnd);

        /******************/
        /** Init Renderer */
        /******************/
        m_Renderer = std::make_unique<D3D11Renderer>(m_Hwnd, m_Data.Width, m_Data.Height);


        /************************************/
        /** Register Mouse Raw input device */
        /************************************/
        RAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01; // mouse page
        rid.usUsage = 0x02;     // mouse usage
        rid.dwFlags = 0;
        rid.hwndTarget = nullptr;
        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
        {
            throw DEVIL_LAST_EXCEPT();
        }
	}


	Window::~Window()
	{
		ShutDown();
	}


    std::optional<int> Window::OnUpdate()
	{
        // ********************************************************************************************
        // Note here that when docking with imgui, 
        // you can also capture messages if you want to drag an imgui window outside of a win32 window
        // The second parameter needs to be set to nullptr!!! 
        // if set to m_Hwnd command to capture the message of the current win32 window
        // ********************************************************************************************
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return static_cast<int>(msg.wParam);
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return {};
	}

    void Window::EnableCursor() noexcept
    {
        m_bCursorEnabled = true;
        ShowCursor();
        EnableImGuiMouse();
        FreeCursor();
    }

    void Window::DisableCursor() noexcept
    {
        m_bCursorEnabled = false;
        HideCursor();
        DisableImGuiMouse();
        ConfineCursor();
    }

    void Window::EnableRawInput() noexcept
    {
        m_bRawInputEnabled = true;
    }

    void Window::DisableRawInput() noexcept
    {
        m_bRawInputEnabled = false;
    }

    void Window::ShowRawInputImGuiWindow() noexcept
    {
        static int x{0}, y{0};
        while (const auto d = ReadRawDelta())
        {
            x += d->x;
            y += d->y;
        }
        if (ImGui::Begin("Raw Input"))
        {
            ImGui::Text("Tally: (%d,%d)", x, y);
        }
        ImGui::End();
    }

	void Window::SetVSync(bool bEnable)
	{
	}


	void Window::SetFullScreen(bool bFullScreen)
	{
	}


    std::optional<Window::RawDelta> Window::ReadRawDelta() noexcept
    {
        if (m_RawDeltaBuffer.empty())
        {
            return std::nullopt;
        }
        const RawDelta d = m_RawDeltaBuffer.front();
        m_RawDeltaBuffer.pop();
        return d;
    }

    LRESULT Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
    {
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lparam);
            Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

            // ****************
            // set user pointer
            // ****************
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
            return pWnd->HandleMsg(hwnd, msg, wparam, lparam);
        }
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }


    LRESULT Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
    {
        Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        return pWnd->HandleMsg(hwnd, msg, wparam, lparam);
    }


    LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
    {
        /** ImGui WndProc Handler */
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
            return true;

        const auto io = ImGui::GetIO();

        switch (msg)
        {
        case WM_ACTIVATE:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            WindowActivateEvent event(wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_CLOSE:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            WindowCloseEvent event;
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            /** Post Quit Message */
            PostQuitMessage(0);

            return 0;
        }

        case WM_SIZE:
        {
            if (wparam == SIZE_MINIMIZED)
                return 0;
            
            unsigned int resizeWidth = (UINT)LOWORD(lparam);
            unsigned int resizeHeight = (UINT)HIWORD(lparam);

            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            WindowResizeEvent event(resizeWidth, resizeHeight);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
        case WM_ENTERSIZEMOVE:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            WindowEnterSizeMoveEvent event;
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

            // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
            // Here we reset everything based on the new window dimensions.
        case WM_EXITSIZEMOVE:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            WindowExitSizeMoveEvent event;
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        /** Keyboard Msg */
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            if (io.WantCaptureKeyboard)
            {
                return 0;
            }

            /** Key Code State */
            m_KeyStates[wparam] = true;

            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            KeyPressedEvent event(wparam, lparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            if (io.WantCaptureKeyboard)
            {
                return 0;
            }

            /** Key Code State */
            m_KeyStates[wparam] = false;

            Window * pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            KeyReleasedEvent event(wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        /** Mouse Msg */
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        {
            if (io.WantCaptureMouse)
            {
                return 0;
            }
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseButtonPressedEvent event(wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            if (io.WantCaptureMouse)
            {
                return 0;
            }
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseButtonReleasedEvent event(wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            if (io.WantCaptureMouse)
            {
                return 0;
            }
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseScrolledEvent event(LOWORD(lparam), HIWORD(lparam));
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            if (io.WantCaptureMouse)
            {
                return 0;
            }
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseMovedEvent event(LOWORD(lparam), HIWORD(lparam), wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }



        /***********************/
        /** RAW MOUSE MESSAGES */
        /***********************/
        case WM_INPUT:
        {
            // need Raw input Enabled!!!
            if (!m_bRawInputEnabled)
            {
                return 0;
            }
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            WindowRawInputEvent event(lparam, wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }


        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                //const int dpi = HIWORD(wParam);
                //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
                const RECT* suggested_rect = (RECT*)lparam;
                ::SetWindowPos(hwnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            return 0;
        }


        /** Default Window Proc */
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }


	void Window::ShutDown()
	{
        DestroyWindow(m_Hwnd);
	}

    void Window::ConfineCursor() noexcept
    {
        RECT rect{};
        GetClientRect(m_Hwnd, &rect);
        MapWindowPoints(m_Hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
        ClipCursor(&rect);
    }

    void Window::FreeCursor() noexcept
    {
        ClipCursor(nullptr);
    }

    void Window::ShowCursor() noexcept
    {
        while (::ShowCursor(true) < 0);
    }

    void Window::HideCursor() noexcept
    {
        while (::ShowCursor(false) >= 0);
    }

    void Window::EnableImGuiMouse() noexcept
    {
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }

    void Window::DisableImGuiMouse() noexcept
    {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }
}