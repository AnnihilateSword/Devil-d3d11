#include "EnginePCH.h"

#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "../resource.h"


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
        cx.hbrBackground = nullptr;
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



        /******************/
        /** Init Renderer */
        /******************/
        m_Renderer = std::make_unique<D3D11Renderer>(m_Hwnd, m_Data.Width, m_Data.Height);
	}


	Window::~Window()
	{
		ShutDown();
	}


    std::optional<int> Window::OnUpdate()
	{
        if (PeekMessage(&msg, m_Hwnd, 0, 0, PM_REMOVE))
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


	void Window::SetVSync(bool bEnable)
	{
	}


	void Window::SetFullScreen(bool bFullScreen)
	{
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
        switch (msg)
        {
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
        }

            // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
            // Here we reset everything based on the new window dimensions.
        case WM_EXITSIZEMOVE:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            WindowExitSizeMoveEvent event;
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);
        }

        /** Keyboard Msg */
        case WM_KEYDOWN:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
            KeyPressedEvent event(wparam, lparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_KEYUP:
        {
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
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseButtonReleasedEvent event(wparam);
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_MOUSEWHEEL:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseScrolledEvent event(LOWORD(lparam), HIWORD(lparam));
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }

        case WM_MOUSEMOVE:
        {
            Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            MouseMovedEvent event(LOWORD(lparam), HIWORD(lparam));
            if (pWnd->GetEventCallback())
                pWnd->GetEventCallback()(event);

            return 0;
        }


        /** Default Window Proc */
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    }


	void Window::ShutDown()
	{
        DestroyWindow(m_Hwnd);
	}
}