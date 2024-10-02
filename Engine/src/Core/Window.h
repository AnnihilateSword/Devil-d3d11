#pragma once

#include "EnginePCH.h"

#include "Core/Core.h"
#include "Core/DevilException.h"
#include "Events/Event.h"

#include "Platform/D3D11/D3D11Renderer.h"

namespace Devil
{
	/** Window Props */
	struct WindowProps
	{
		WindowProps(char* title = "Devil Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}

		char* Title{};
		unsigned int Width{};
		unsigned int Height{};
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		/** Devil Exception */
		class Exception : public DevilException
		{
		public:
			Exception(const char* file, int line, HRESULT hr) noexcept;

		public:
			virtual const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;
			static std::string TranslateErrorCode(HRESULT hr);
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;

		private:
			HRESULT m_Hr;
		};

	public:
		/** WindowClass */
		class WindowClass
		{
		public:
			WindowClass() noexcept;
			virtual ~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;

		public:
			static const char* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;

		private:
			static constexpr const char* s_WndClassName = "DevilEngine";
			static WindowClass s_WndClass;
			HINSTANCE m_hInstance;
		};

	public:
		struct RawDelta
		{
			int x, y;
		};

	public:
		/**************************/
		/** Constructor Functions */
		/**************************/
		Window(const WindowProps& props = WindowProps());
		virtual ~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	public:
		/** Logic Functions */
		virtual std::optional<int> OnUpdate();

		void EnableCursor() noexcept;
		void DisableCursor() noexcept;
		void EnableRawInput() noexcept;
		void DisableRawInput() noexcept;

		void ShowRawInputImGuiWindow() noexcept;

		/** Getter and Setter */
		inline virtual void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		inline virtual EventCallbackFn& GetEventCallback() { return m_Data.EventCallback; }
		virtual void SetWidth(unsigned int width) { m_Data.Width = width; }
		virtual void SetHeight(unsigned int height) { m_Data.Height = height; }
		virtual void SetVSync(bool bEnable);
		virtual void SetFullScreen(bool bFullScreen);
		virtual void SetInWindow(bool bInWindow) { m_bInWindow = bInWindow; }

		virtual void* GetNativeWindow() const { return m_Hwnd; }
		virtual unsigned int GetWidth() const { return m_Data.Width; }
		virtual unsigned int GetHeight() const { return m_Data.Height; }
		virtual bool IsVSync() const { return m_Data.bVSync; }
		virtual bool IsFullScreen() const { return m_Data.bFullScreen; }
		virtual bool IsCursorEnabled() const { return m_bCursorEnabled; }
		virtual bool IsInWindow() const { return m_bInWindow; };
		virtual bool IsRawInputEnabled() const { return m_bRawInputEnabled; };

		virtual D3D11Renderer& GetRenderer() const { return *m_Renderer; }

		std::optional<RawDelta> ReadRawDelta() noexcept;

		// Mouse
		virtual bool KeyIsPressed(unsigned char keycode) const noexcept { return m_KeyStates[keycode]; }

	private:
		/** [STATIC] Member WndProc */
		static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

		/** Cleanup */
		virtual void ShutDown();

	public:
		// Fixed mouse!!!
		void ConfineCursor() noexcept;
		// Free mouse!!!
		void FreeCursor() noexcept;
		void ShowCursor() noexcept;
		void HideCursor() noexcept;

	private:
		void EnableImGuiMouse() noexcept;
		void DisableImGuiMouse() noexcept;

	public:
		std::vector<BYTE> m_RawBuffer;
		std::queue<RawDelta> m_RawDeltaBuffer;

		// *****
		// mouse
		// *****
		unsigned char m_KeyStates[256]{ 0 };

	private:
		struct WindowData
		{
			char* Title{ nullptr };
			unsigned int Width{};
			unsigned int Height{};
			bool bVSync{ true };
			bool bFullScreen{ false };

			EventCallbackFn EventCallback{};
		};
		WindowData m_Data{};

		HWND m_Hwnd{};
		MSG msg{};


		/*************/
		/** Renderer */
		/*************/
		std::unique_ptr<D3D11Renderer> m_Renderer{ nullptr };

		/** Properties */
		// Input
		bool m_bCursorEnabled{ true };
		bool m_bInWindow{ false };
		bool m_bRawInputEnabled{ false };
	};



// ****************
// Exception Macros
// ****************
#define DEVIL_EXCEPT(hr) Window::Exception(__FILE__, __LINE__, hr)
#define DEVIL_LAST_EXCEPT() Window::Exception(__FILE__, __LINE__, GetLastError())
}