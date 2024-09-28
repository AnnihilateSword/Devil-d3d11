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
			unsigned int width = 800,
			unsigned int height = 450)
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


		/** Getter and Setter */
		inline virtual void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		inline virtual EventCallbackFn& GetEventCallback() { return m_Data.EventCallback; }
		virtual void SetVSync(bool bEnable);
		virtual void SetFullScreen(bool bFullScreen);

		virtual void* GetNativeWindow() const { return m_Hwnd; }
		virtual unsigned int GetWidth() const { return m_Data.Width; }
		virtual unsigned int GetHeight() const { return m_Data.Height; }
		virtual bool IsVSync() const { return m_Data.bVSync; }
		virtual bool IsFullScreen() const { return m_Data.bFullScreen; }

		virtual D3D11Renderer& GetRenderer() const { return *m_Renderer; }

	private:
		/** [STATIC] Member WndProc */
		static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

		/** Cleanup */
		virtual void ShutDown();

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
	};



// ****************
// Exception Macros
// ****************
#define DEVIL_EXCEPT(hr) Window::Exception(__FILE__, __LINE__, hr)
#define DEVIL_LAST_EXCEPT() Window::Exception(__FILE__, __LINE__, GetLastError())
}